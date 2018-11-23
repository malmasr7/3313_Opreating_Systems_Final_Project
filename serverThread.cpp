

#include "thread.h"
#include "socketserver.h"
#include <vector>
#include "Pair.h"

#include "serverThread.h"

// This thread handles the server operations
class ServerThread : public Thread
{

public:

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