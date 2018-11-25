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



//Socket &socket

static std::vector<Socket> mySockets1; 

int i=0;

class pairs {





public: 



// pairs();


void setPair(int myIndex,int myPartnersIndex){

//sets the chat indexes of the first 2 clients 

}

int getPair(){

//returns index of the partner

}

};



//static std::vector<Socket&> mySockets3; 



//std::vector<Socket> &mySockets2; 



//std::vector<Socket&> &mySockets4; 



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

std::cout<<"Socket 1: "<<mySockets1[0]<<'\n';

std::cout<<"Socket 2: "<<mySockets1[1]<<'\n';

std::cout<<"Socket: "<<socket<<'\n';

// Wait for data

int readError =socket.Read(data);



if(readError==0) {

std::cout<< "\nThere was an error Reading\n";

return -1;

}

//***************************EDIT HERE***************************//

// Perform operations on the data

std::string data_str = data.ToString();

//std::reverse(data_str.begin(), data_str.end());

//NO LONGER NEED TO REVERSE ORDER OF DATA 

data = ByteArray(data_str);



//**//*SEND DATA TO OTHER CLIENT, NOT BACK TO SAME CLIENT*/

//***commented out int writeError = socket.Write(data);



int newWrite1;





if(1/*socket==mySockets1[1]*/){

newWrite1 = mySockets1[0].Write(data);

//i=0;



if (newWrite1==-1){

std::cout<<"\nThere was an error writing\n";

return -1;

}

}

//my index is odd )

//index sending to is my index +1

///////

// if(mySockets1[0]==socket){

// newWrite1 = mySockets1[1].Write(data);

// //i=1;

// if (newWrite1==-1){

// std::cout<<"\nThere was an error writing\n";

// return -1;

// }

// }

///////

//my index is even )

//index sending to is my index -1





//THE INDEX TO SEND TO IS ONE ABOVE OR ONE BELOW ITS OWN INDEX O

//int writeError = socket.Write(data);



//int newWrite2 = mySockets3[0].Write(data);




//REFERENCE OUR MATCHING SOCKET HERE 

// Socket& toClose = thread->GetSocket();

// toClose.Close();

//**************************************************//




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



//***************************************************************//

        //Make sure you CLOSE all socket at the end

// Pass a reference to this pointer into a new socket thread


Socket& socketReference = *newConnection;

//mySockets1.insert(threadNum,socketReference)



std:: cout<< "\n The array is now being added to \n";

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

std::cout<<"\nUnable to bind to requested Port\n";

return -1;

} 

    

    //Cleanup, including exiting clients, when the user presses enter

}