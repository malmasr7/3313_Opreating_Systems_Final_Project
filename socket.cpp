#include <strings.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>

#include "socket.h"
namespace Sync{
	
Socket::Socket(std::string const & ipAddress, unsigned int port)
    : Blockable(),open(false)
{
    // First, call socket() to get a socket file descriptor
    SetFD(socket(AF_INET, SOCK_STREAM, 0));
    if (GetFD() < 0)
        throw std::string("Unable to initialize socket server");

    // Start by zeroing out the socket descriptor
     bzero((char*)&socketDescriptor,sizeof(sockaddr_in));

    // Now try to map the IP address, as provided, into the socket Descriptor
    if (!inet_aton(ipAddress.c_str(),&socketDescriptor.sin_addr))
        throw std::string("IP Address provided is invalid");
    socketDescriptor.sin_family = AF_INET;
    socketDescriptor.sin_port = htons(port);
}

Socket::Socket(int sFD)
    : Blockable(sFD)
{
    open = true;
}

Socket::Socket(Socket const & s)
    :Blockable(s)
{    
    open = s.open;
}

Socket & Socket::operator=(Socket const & rhs)
{
    close(GetFD());
    socketDescriptor = rhs.socketDescriptor;
    SetFD(dup(rhs.GetFD()));
    open = rhs.open;
}

Socket::~Socket(void)
{
    Close();
}


int Socket::Open(void)
{
    int connectReturn = connect(GetFD(),(sockaddr*)&socketDescriptor,sizeof(socketDescriptor));
    if (connectReturn != 0)
    {
        throw std::string("Unable to open connection");
    }
    open = true;
}

int Socket::Write(ByteArray const & buffer)
{
    if (!open)
        return -1;
    char * raw = new char[buffer.v.size()];
    for (int i=0;i<buffer.v.size();i++)
        raw[i] = buffer.v[i];
    int returnValue = write(GetFD(),raw,buffer.v.size());
    if (returnValue <=0)
        open = false;
    return returnValue;
}

static const int MAX_BUFFER_SIZE = 256;
int Socket::Read(ByteArray & buffer)
{
    char raw[MAX_BUFFER_SIZE];
    if (!open)
        return 0;

    buffer.v.clear();
    // Allow interruption of block.
    FlexWait waiter(2,this,&terminator);
    Blockable * result = waiter.Wait();
    // This happens if the call was shutdown on this side
    if (result == &terminator)
    {
        terminator.Reset();
        return 0;
    }
    // If we got here, we need to read the socket
    // Messages greater than MAX_BUFFER_SIZE are not handled gracefully.
    ssize_t received = recv(GetFD(), raw, MAX_BUFFER_SIZE, 0);
    for (int i=0;i<received;i++)
        buffer.v.push_back(raw[i]);
    if (received <=0)
        open = false;
    return received;
}

void Socket::Close(void)
{
    close(GetFD());
    shutdown(GetFD(),SHUT_RDWR);
    open = false;
    terminator.Trigger();

}
};