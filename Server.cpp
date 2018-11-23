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

#include "socketThread.h"
#include "serverThread.h"
#include "Pair.h"

using namespace Sync;

//put it in scope
//Locate function to find which vector location has thread
//========================================================###########
    //--after can just call get pair (check full, and empty first)
    //--locate's thread's pair in the vector if given a thread
int locateThreadPair(SocketThread &input){
    int location = -1;  //can use for error check. if -1 then could not find
    for(int index = 0; index < socketThreadPairs.size(); index++){
        if( (socketThreadPairs[index].getLeft() == input) || (socketThreadPairs[index].getRight() == input) ){    //comapred by addresses left or right
            location = index; 
            break;
        }
    }
    return location;
}
//========================================================

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
