/*
	title:		SE3313 Lab 4 Solution Edited
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

//put it in scope
//Locate function to find which vector location has thread
//========================================================###########
    //--after can just call get pair (check full, and empty first)
    //--locate's thread's pair in the vector if given a thread
    int locateThreadPair(SocketThread &input){
        int location = -1;  //can use for error check. if -1 then could not find
        for(int index = 0; index < ServerThread::socketThreadPairs.size(); index++){
            if( ( ServerThread::socketThreadPairs[index].getLeft() == input) || ( ServerThread::socketThreadPairs[index].getRight() == input) ){    //comapred by addresses left or right
                location = index; 
                break;
            }
        }
        return location;
    }
//========================================================

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

                /*
                // Perform operations on the data
                std::string data_str = data.ToString();
                std::reverse(data_str.begin(), data_str.end());
                data = ByteArray(data_str);
                */

                //========================================#######
                // Send it back (to own pair reference)
                //socket'sPair.Write(data);
                int placement = locateThreadPair(socket);    //find location of this referenced socket in the vector
                socketThreadPairs[placement].getPartner(socket).Write(data);       //in pair, get socket's partner, and write the data message to this partner
                //========================================
            }
            catch (...)
            {
                // We catch the exception, but there is nothing for us to do with it here. Close the thread.
            }
        }

        return 0;
    }
};

//Struct for pairs
//===========================
//custom pair class, avoided pair::pair from c++ for custom call functions
class Pair{
    public:
        SocketThread* left;
        SocketThread* right;

        //constructor
        Pair(){
            left = NULL;
            right = NULL;
        }

        //destructor
        ~Pair() { }

        //--Don't know if necessary {========
        //                                  =
        //get right                         =
        //--returns address of thread       =
        &SocketThread getRight(){         //=
            return *right;                //=
        }                                 //=
                                          //=
        //get left                          =
        //--returns address of thread       =
        &SocketThread getLeft(){          //=
            return *left;                 //=
        }                                 //=
        //}==================================

        //get other
        //--return thread pointed at (or returns address of thread pointed at?)
        //needed here?
        SocketThread getPartner(SocketThread &input){
            if (this.isFull()==true){   //only runs if has a partner
                if(input == left){
                    return * right;  //if the address of input and address of object pointed at by left are equal, return right object
                }
                if(input == right){
                    return * left;    
            }       //needs to be formatted better. What if it doesn't match?
        }

        //is empty?
        bool isEmpty(){     //never be just right side and left side empty
            if(*left==NULL){    return true;    }
            else{   return false;   }
        }

        //is full?
        bool isFull(){
            if( (*left != NULL) && (*right != NULL) ){    return true;      }
            else{   return false;   }
        }

        //add 
        //--implement logic to ensure is full is not true on enter from vector
        //--vector, call empty. If empty returns false, then always
        //--adds to the right side. else add to the left
        void Add(SocketThread &input){      //pass Thread to this function to use
            if(isEmpty()==false){    right = input;     }        //not empty, not full, only open on right   ///only want to pass location, not copy it ########### Needs change
            else{   left = input;  }                            //value in left = address of the input
        }   
        //^not sure if getting the correct value
    }
}
//============================


// This thread handles the server operations
class ServerThread : public Thread
{
private:
    SocketServer& server;

    //====================================#######
    //old vector
    //std::vector<SocketThread*> socketThreads;
    //new vector
    std::vector<Pair> socketThreadPairs;
    //====================================

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

    ~ServerThread()
    {
        // Close the client sockets
        for (auto pair : socketThreadPairs)
        {
            try
            {
                //==============================#######
                // Close the sockets of the pair
                if (pair.isFull()==true){   //close both, full fair
                    //close left
                    Socket& toClose1 = pair->getLeft();
                    toClose1.Close();
                    //close right
                    Socket& toClose2 = pair->getRight();
                    toClose2.Close();
                }
                if (pair.isEmpty()==true){ 
                    continue;   //no threads to close, next spot in vector
                }
                else{   //only left has thread
                    Socket& toClose = pair->getLeft();
                    toClose.Close();
                }
                //==============================
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
                
                //===============================================###########
                //-->First Pair unique case
                //New push back
                //check if last pair is full socketThreadPairs[socketThreadPairs.size()].isFull
                //      -create new pair,
                //      -add to pair and append to vector (push_back)
                //OR
                //add to avaliable pair in vector. at vector[vector.size()] (add to last pair)
                
                //Original
                //socketThreads.push_back(new SocketThread(socketReference, terminate));

                //if last full is not full
                if (  socketThreadPairs[socketThreadPairs.size()].isFull() == false  ){
                    socketThreadPairs[socketThreadPairs.size()].Add(new SocketThread(socketReference, terminate));          //add new socketThread to that pair
                }
                else{   //logic for new pair, pair is full, need a new pair to add to
                    socketThreadPairs.push_back( new Pair() );                                                              //add new pair to end
                    socketThreadPairs[socketThreadPairs.size()].Add(new SocketThread(socketReference, terminate));          //new last pair is new pair, give it a new socketThread
                }
                //===============================================
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
    //std::cout << "SE3313 Lab 4 Server" << std::endl;
    //================#######
    std::cout << "SE3313 Chat Server" <<std::endl;
    //================
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
