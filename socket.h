#ifndef SOCKET_H
#define SOCKET_H
#include <vector>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

#include "Blockable.h"
namespace Sync{
	
class ByteArray
{
public:
    std::vector<char> v;
    std::string ToString(void) const
    {
        std::string returnValue;
        for (int i=0;i<v.size();i++)
            returnValue.push_back(v[i]);
        return returnValue;
    }
    ByteArray(void){}
    ByteArray(std::string const & in)
    {
        for (int i=0;i<in.size();i++)
            v.push_back(in[i]);
    }
    ByteArray(void * p, int s)
    {
        char * temp = (char*)p;
        for (int i=0;i<s;i++)
            v.push_back(temp[i]);
    }
};

class Socket : public Blockable
{
private:
    sockaddr_in socketDescriptor;
    bool open;
    Event terminator;
public:
    Socket(std::string const & ipAddress, unsigned int port);
    Socket(int socketFD);
    Socket(Socket const & s);
    Socket & operator=(Socket const & s);
    ~Socket(void);

    int Open(void);
    int Write(ByteArray const & buffer);
    int Read(ByteArray & buffer);
    void Close(void);
};
};
#endif // SOCKET_H
