/*
	title:		SE3313 Lab 4 Solution
	author: 	Daniel Bailey (dbaile7@uwo.ca)
	date:		November 15, 2018
	brief:
		This is the accepted TA solution for lab 4. There are many
		different ways to complete this task so yours may look completely
		different. If you have any questions about what I did or why
		I did something, don't hesitate to ask (Email is up top).
*/

#include "thread.h"
#include "socketserver.h"
#include <stdlib.h>
#include <algorithm>
#include <time.h>
#include <list>
#include <vector>

using namespace Sync;

// This thread handles each client connection
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
    SocketThread(Socket& socket, bool& terminate)
    : socket(socket), terminate(terminate)
    {}

    ~SocketThread()
    {}

    Socket& GetSocket()
    {
        return socket;
    }

    virtual long ThreadMain()
    {
        // If terminate is ever flagged, we need to gracefully exit
        while(!terminate)
        {
            try
            {
                // Wait for data
                socket.Read(data);

                // Perform operations on the data
                std::string data_str = data.ToString();
                std::reverse(data_str.begin(), data_str.end());
                data = ByteArray(data_str);

                // Send it back
                socket.Write(data);
            }
            catch (...)
            {
                // We catch the exception, but there is nothing for us to do with it here. Close the thread.
            }
        }

        return 0;
    }
};

// This thread handles the server operations
class ServerThread : public Thread
{
private:
    SocketServer& server;
    std::vector<SocketThread*> socketThreads;
    bool terminate = false;
public:
    ServerThread(SocketServer& server)
    : server(server)
    {}

    ~ServerThread()
    {
        // Close the client sockets
        for (auto thread : socketThreads)
        {
            try
            {
                // Close the socket
                Socket& toClose = thread->GetSocket();
                toClose.Close();
            }
            catch (...)
            {
                // If already closed, this will cause an exception
            }
        }

        // Terminate the thread loops
        terminate = true;
    }

    virtual long ThreadMain()
    {
        while(true)
        {
            try
            {
                // Wait for a client socket connection
                Socket* newConnection = new Socket(server.Accept());

                // Pass a reference to this pointer into a new socket thread
                Socket& socketReference = *newConnection;
                socketThreads.push_back(new SocketThread(socketReference, terminate));
            }
            catch (TerminationException terminationException)
            {
                return terminationException;
            }
            catch (std::string error)
            {
                std::cout << std::endl << "[Error] " << error << std::endl;
                return 1;
            }
        }
    }
};

int main(void)
{
    // Welcome the user
    std::cout << "SE3313 Lab 4 Server" << std::endl;
    std::cout << "Press enter to terminate the server...";
    std::cout.flush();

    // Create our server
    SocketServer server(3000);    

    // Need a thread to perform server operations
    ServerThread serverThread(server);

    // This will wait for input to shutdown the server
    FlexWait cinWaiter(1, stdin);
    cinWaiter.Wait();
    std::cin.get();

    // Shut down and clean up the server
    server.Shutdown();
    return 0;
}
