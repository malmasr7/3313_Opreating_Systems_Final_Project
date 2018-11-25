// #include "thread.h"
// #include "socketserver.h"
// #include <stdlib.h>
// #include <time.h>
// #include <list>
// #include <vector>
// #include <algorithm>

// using namespace Sync;
// // This thread handles each client connection
// class SocketThread : public Thread
// {
// private:
//     // Reference to our connected socket
//     Socket& socket;
//     // The data we are receiving
//     ByteArray data;
// public:
//     SocketThread(Socket& socket)
//     : socket(socket)
//     {}

//     ~SocketThread()
//     {}

//     Socket& GetSocket()
//     {
//         return socket;
//     }

//     virtual long ThreadMain()
//     {
//         while(1)
//         {
//             try
//             {
//                 // Wait for data
//                 socket.Read(data);

//                 // Perform operations on the data
//                 std::string data_str = data.ToString();
//                 std::reverse(data_str.begin(), data_str.end());
//                 data = ByteArray(data_str);

//                 // Send it back
//                 socket.Write(data);
//             }
//             catch (...)
//             {
//                 // We catch the exception, but there is nothing for us to do with it here. Close the thread.
//             }
//         }
		
// 		// Need to gracefully exit

//         return 0;
//     }
// };

// // This thread handles the server operations
// class ServerThread : public Thread
// {
// private:
//     SocketServer& server;
//     bool terminate = false;
// public:
//     ServerThread(SocketServer& server)
//     : server(server)
//     {}

//     ~ServerThread()
//     {
//         // Cleanup
//     }

//     virtual long ThreadMain()
//     {
//         // Wait for a client socket connection
//         Socket* newConnection = new Socket(server.Accept());
// 		//Make sure you CLOSE all socket at the end
//         // Pass a reference to this pointer into a new socket thread
//         Socket& socketReference = *newConnection;
//         SocketThread* socketThread = new SocketThread(socketReference);
//     }
// };


// int main(void)
// {
//     std::cout << "I am a server." << std::endl;
// 	std::cout << "Press enter to terminate the server...";
//     std::cout.flush();
	
// 	// Create our server
//     SocketServer server(3000);    

//     // Need a thread to perform server operations
//     ServerThread serverThread(server);
	
// 	// This will wait for input to shutdown the server
//     FlexWait cinWaiter(1, stdin);
//     cinWaiter.Wait();
//     std::cin.get();

//     // Shut down and clean up the server
//     server.Shutdown();
	
// 	//Cleanup, including exiting clients, when the user presses enter
// }
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

static std::vector<Socket> mySockets1; 
// int i=0;
// class pairs {

//     public: 
    
//     void setPair(int myIndex,int myPartnersIndex){
//         //sets the chat indexes of the first 2 clients 
//     }
//     int getPair(){
//         //returns index of the partner
//     }
// };


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
                std::cout<<"Vector size: "<<mySockets1.size()<<'\n';
                //only output these values if there are two clients
                if(mySockets1.size()>=2){
                std::cout<<"Socket 1: "<<mySockets1[0]<<'\n';
                std::cout<<"Socket 2: "<<mySockets1[1]<<'\n';
                std::cout<<"Socket: "<<socket<<'\n'<<'\n'; 
                }
                
//client 1 --> server socket
//client 2 --> server socket 2


                // Wait for data
                int readError =socket.Read(data);

                if(readError==0) {
                    std::cout<< "\nThere was an error Reading\n";
                    return -1;
                }
                // Perform operations on the data
                std::string data_str = data.ToString();

                data = ByteArray(data_str);

                //**//*SEND DATA TO OTHER CLIENT, NOT BACK TO SAME CLIENT*/
                //int writeError = socket.Write(data);

                int newWrite1;

                if(1/*socket==mySockets1[1]*/){
                //****Telling the current socket where to write**EDIT HERE**//
                newWrite1 =  mySockets1[0].Write(data);
                    //i=0;

                    if (newWrite1==-1){
                   std::cout<<"\nThere was an error writing\n";
                        return -1;
                    }
                }
                //if index is odd 
                //send to own vector index-1
                //if index is even 
                //send to own vector index+1

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
        
        //************************EDIT HERE************************//
        //**//*WHEN A CONNECTION IS ESTABLISHED, PAIR THREAD WITH ANOTHER THREAD*/

        // Pass a reference to this pointer into a new socket thread
        
        Socket& socketReference = *newConnection;
        //mySockets1.insert(threadNum,socketReference)

        std:: cout<< "\n The array is now being added to \n";
        
        //ADDING TO THE VECTOR OF SOCKETS 
        mySockets1.push_back(socketReference);
        //mySockets3.push_back(socketReference);

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
        std::cout<<"\n Unable to bind to requested Port\n";
        return -1;
    }    
	
	//Cleanup, including exiting clients, when the user presses enter
}



//*LAB 4 GITHUB SOLUTION CODE*//
/*
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
*/
//*lab 4 github original code*//
/*
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
*/