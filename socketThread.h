#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include "socket.h"

class SocketThread : public Thread
{
private:

    // Reference to our connected socket
    Socket& socket;
    // The data we are receiving
    ByteArray data;
    // Are we terminating?
    bool& terminate;

public:

    SocketThread(Socket& socket, bool& terminate): socket(socket), terminate(terminate){}
    ~SocketThread(){}
    Socket& GetSocket();
    virtual long ThreadMain();
};

#endif