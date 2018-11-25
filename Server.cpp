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
static std::vector<Socket> matchingServerSockets; 

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
                std::cout<<"Clients Vector size: "<<mySockets1.size()<<'\n';
                std::cout<<"Servers Vector size: "<<matchingServerSockets.size()<<'\n';
                
                std::cout<<"Client Socket 1: "<<mySockets1[0]<<'\n';
                std::cout<<"Client Socket 2: "<<mySockets1[1]<<'\n';
                

                //std::cout<<"current socket: "<<socket<<'\n';
                
               // if(matchingServerSockets.size()>=2){
               // std::cout<<"second socket in array: "<<matchingServerSockets[1]<<'\n';  
               // }
               // std::cout<<"third socket in array: "<<matchingServerSockets[2]<<'\n';
               // std::cout<<"fourth socket in array: "<<matchingServerSockets[3]<<'\n';
                
                // std::cout<<"second socket in array: "<<matchingServerSockets[1]<<'\n';
                // std::cout<<"third socket in array: "<<matchingServerSockets[2]<<'\n';
                // std::cout<<"fourth socket in array: "<<matchingServerSockets[3]<<'\n';

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

                static std::vector<Socket> serverSocketsVector; 
                static std::vector<int> serverIntSockets;
                
                serverSocketsVector.push_back(socket);
                

                //only add to vector if socket is unique(not in vector)
                if(std::find(serverIntSockets.begin(), serverIntSockets.end(), socket) != serverIntSockets.end()) {
                    /* v contains x */
                } else {
                    /* v does not contain x */
                    std::cout<<"*******************HERE IS THE CURRENT SOCKET: "<<socket<<'\n';
                    std::cout<<"CURRENT SOCKET being added to array: "<<socket<<'\n';
                    serverIntSockets.push_back((socket));//int array 
                    //std::cout<<"socket array is being added to! "<<'\n';
                    //std::cout<<"Server socket being added to aray is: "<<socket<<'\n'<<'\n';
                    //matchingServerSockets.push_back(socket);
                   // std::cout<<"first socket in array: "<<matchingServerSockets[0]<<'\n';
                }
 
                if(serverSocketsVector.size()==1){
                    std::cout<<"First server socket: "<<serverSocketsVector[0]<<'\n';

                    std::cout<<"Second server INT socket: "<<serverIntSockets[0]<<'\n';
                }
                
                if(serverSocketsVector.size()==2){
                    std::cout<<"First server socket: "<<serverSocketsVector[0]<<'\n';
                    std::cout<<"Second server socket: "<<serverSocketsVector[1]<<'\n';
                     
                     std::cout<<"First INT server socket: "<<serverIntSockets[0]<<'\n';
                     std::cout<<"Second INT server socket: "<<serverIntSockets[1]<<'\n';
                }
           
                if(/*socket==11*/socket==serverIntSockets[0]||socket==serverIntSockets[1]){
                //****Telling the current socket where to write**EDIT HERE**//
                if(socket==serverIntSockets[1]){
                       newWrite1 =  mySockets1[1].Write(data);

                    if (newWrite1==-1){
                   std::cout<<"\nThere was an error writing\n";
                        return -1;
                    }
                }
                if(socket==serverIntSockets[0]){
                        newWrite1 =  mySockets1[0].Write(data);

                    if (newWrite1==-1){
                   std::cout<<"\nThere was an error writing\n";
                        return -1;
                    }
                }

                }
                //  if(/*socket==22*/socket==serverIntSockets[1]){
                // //****Telling the current socket where to write**EDIT HERE**//
                // newWrite1 =  mySockets1[1].Write(data);

                //     if (newWrite1==-1){
                //    std::cout<<"\nThere was an error writing\n";
                //         return -1;
                //     }
                // }

//*************************///

//    if(/*socket==22*/socket==serverIntSockets[2]){
//                 //****Telling the current socket where to write**EDIT HERE**//
//                 newWrite1 =  mySockets1[2].Write(data);

//                     if (newWrite1==-1){
//                    std::cout<<"\nThere was an error writing\n";
//                         return -1;
//                     }
//                 }

//                    if(/*socket==22*/socket==serverIntSockets[3]){
//                 //****Telling the current socket where to write**EDIT HERE**//
//                 newWrite1 =  mySockets1[3].Write(data);

//                     if (newWrite1==-1){
//                    std::cout<<"\nThere was an error writing\n";
//                         return -1;
//                     }
//                 }

//*************************///


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

        std:: cout<< "\n The client array of sockets is now being added to \n";
        
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






