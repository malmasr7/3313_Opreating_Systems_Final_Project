#include "thread.h"
#include "socketserver.h"
#include <stdlib.h>
#include <time.h>
#include <list>
#include <vector>
#include <algorithm>

using namespace Sync;
// This thread handles each client connection
class SocketThread : public Thread
{
private:
    // Reference to our connected socket
    Socket& socket;
    // The data we are receiving
    ByteArray data;
public:
    SocketThread(Socket& socket)
    : socket(socket)
    {}

    ~SocketThread()
    {}

    Socket& GetSocket()
    {
        return socket;
    }

    virtual long ThreadMain()
    {
        while(1)
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
		
		// Need to gracefully exit

        return 0;
    }
};

// This thread handles the server operations
class ServerThread : public Thread
{
private:
    SocketServer& server;
    bool terminate = false;
public:
    ServerThread(SocketServer& server)
    : server(server)
    {}

    ~ServerThread()
    {
        // Cleanup
    }

    virtual long ThreadMain()
    {
        // Wait for a client socket connection
        Socket* newConnection = new Socket(server.Accept());
		//Make sure you CLOSE all socket at the end
        // Pass a reference to this pointer into a new socket thread
        Socket& socketReference = *newConnection;
        SocketThread* socketThread = new SocketThread(socketReference);
    }
};


int main(void)
{
    std::cout << "I am a server." << std::endl;
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
	
	//Cleanup, including exiting clients, when the user presses enter
}
