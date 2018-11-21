#include "thread.h"
#include "socketserver.h"
#include <stdlib.h>
#include <time.h>
#include <vector>
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
 bool terminate = false;
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
        while(!terminate)
        {
            try
            {
                // Wait for data
                int readError =socket.Read(data);

                if(readError==0) {
                    std::cout<< "\nThere was an error Reading\n";
                    return -1;
                }

                // Perform operations on the data
                std::string data_str = data.ToString();
                std::reverse(data_str.begin(), data_str.end());
                data = ByteArray(data_str);

                // Send it back
               int writeError= socket.Write(data);

               if (writeError==-1){
                   std::cout<<"\nThere was an error writing\n";
                   return -1;
               }
            }
            catch (...)
            {
                // We catch the exception, but there is nothing for us to do with it here. Close the thread.
            }
        }
		
		// Need to gracefully exit
        

        
    }
};

// This thread handles the server operations
class ServerThread : public Thread
{
private:
    SocketServer& server;
    int threadNum=0;
public:

   bool terminate = false;
    ServerThread(SocketServer& server)
    : server(server)
    {}

    ~ServerThread()
    {
        // Cleanup
    }

    virtual long ThreadMain()
    { 
        std::vector <SocketThread*> myThreads;
        while (true){
        // Wait for a client socket connection
        Socket* newConnection = new Socket(server.Accept());
         if (terminate){
        break;
        }
        std:: cout<< "\n Connection Established \n";
       
		//Make sure you CLOSE all socket at the end
        // Pass a reference to this pointer into a new socket thread
        Socket& socketReference = *newConnection;
        threadNum++;

        SocketThread* socketThread = new SocketThread(socketReference);
        myThreads.push_back(socketThread);
        } 

        for (int i=0; i<threadNum; i++){
		myThreads[i]-> terminate= true; 
	}

	for (int i=0; i<(threadNum); i++){
		delete myThreads[i]; 
	}
    return 0;


    }
};


int main(void)
{
    std::cout << "I am a server." << std::endl;
	std::cout << "Press enter to terminate the server...";
    std::cout.flush();
	
	// Create our server
    try{
    SocketServer server(3000); 
    ServerThread serverThread(server);
    // This will wait for input to shutdown the server
    FlexWait cinWaiter(1, stdin);
    cinWaiter.Wait();
    std::cin.get();

    serverThread.terminate==true;

    // Shut down and clean up the server
    server.Shutdown();


    } catch(...){
        std::cout<<"\nUnable to bind to requested Port\n";
        return -1;
    }   

    
	
	
	
	//Cleanup, including exiting clients, when the user presses enter
}


