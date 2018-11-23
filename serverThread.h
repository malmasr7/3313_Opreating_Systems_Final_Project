#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include "socketserver.h"
#include "thread.h"
#include "Pair.h"
#include <vector>


// This thread handles the server operations
class ServerThread : public Thread
{
private:
    SocketServer& server;
    vector<Pair> socketThreadPairs;
    bool terminate = false;

public:

    ServerThread(SocketServer& server)
    : server(server)
    {
    //================##########
    //--create first pair so not empty vector on serverThread initiation
        socketThreadPairs.push_back(new Pair());
    //================
    }

    ~ServerThread();

    virtual long ThreadMain();
};

#endif