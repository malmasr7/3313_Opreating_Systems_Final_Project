#include "thread.h"
#include "socket.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
//#include <curses.h>
#include<unistd.h>

//#include <conio.h>

using namespace Sync;


//WORKS TO HERE
//WORKS TO HERE
//WORKS TO HERE
//WORKS TO HERE
//WORKS TO HERE
//WORKS TO HERE
//WORKS TO HERE
//WORKS TO HERE
//WORKS TO HERE
//WORKS TO HERE
/*

only windows--
if(GetKeyState(E) & 0x8000){
std::cout<<"E is being pressed"<<'\n';
}

unix version-


*/



int main(void){
		std::cout << "SE3313 Lab 4 Client" << std::endl;
		
		Socket socket("52.24.128.64", 3005);
		//Socket socket("127.0.0.1", 3000);
	
		ByteArray data;
		try{

		int result = socket.Open();
		} catch(...){
			std::cout<< "\nThere was an error opening\n";
			return -1;
		}

		std::cout << "Welcome to the chat room!\n";

		static int i=0;
		//sleep(2);
		
		//THIS IS THE LOOP
		while(true){

			std::string data_str;
			std::cout << "Please input your data (done to exit): ";
			std::cout.flush();

			// Get the data
			std::getline(std::cin, data_str);

			//read for 10 seconds 
			if(data_str == "r"){
				while(1){
					//reading // Get the response
					int ReadError=socket.Read(data);
					if (ReadError==0){
						std::cout<<"\nServer Socket is closed\n";
						return -1;
					}
					data_str = data.ToString();
					std::cout << "Server Response: " << data_str << std::endl; 
							
					i++;
					//sleep(1);
					if(i==1){
						i=0;
						break;
					}
				}
			}

			//go back to write
			
			if (data_str == "done")
				{ break;}
			
			data = ByteArray(data_str);

			// Write to the server
			int WriteError=socket.Write(data);

			if (WriteError==-1){
				std::cout<<"\n Server Socket is closed.\n";
				return -1;
			}
			
			// // Get the response
			// int ReadError=socket.Read(data);
			// if (ReadError==0){
			// 	std::cout<<"\nServer Socket is closed\n";
			// 	return -1;
			// }
			// data_str = data.ToString();
			// std::cout << "Server Response: " << data_str << std::endl;  
		}

	socket.Close();
	return 0;
}

//thread will read, in main you will indicate when you want to 
//write, once writing is done, allow the reader to continue.


































//*LAB 4 GITHUB solution CODE*/
/*
#include "thread.h"
#include "socket.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace Sync;

// This thread handles the connection to the server
class ClientThread : public Thread
{
private:
	// Reference to our connected socket
	Socket& socket;

	// Reference to boolean flag for terminating the thread
	bool& terminate;

	// Are we connected?
	bool connected = false;

	// Data to send to server
	ByteArray data;
	std::string data_str;
	int expectedLength = 0;
public:
	ClientThread(Socket& socket, bool& terminate)
	: socket(socket), terminate(terminate)
	{}

	~ClientThread()
	{}

	void TryConnect()
	{
		try
		{
			std::cout << "Connecting...";
			std::cout.flush();
			socket.Open();
			connected = true;
			std::cout << "OK" << std::endl;
		}
		catch (std::string exception)
		{
			std::cout << "FAIL (" << exception << ")" << std::endl;
			return;
		}
	}

	virtual long ThreadMain()
	{
		// Initially we need a connection
		while (true)
		{
			// Attempt to connect
			TryConnect();

			// Check if we are exiting or connection was established
			if (terminate || connected)
			{
				break;
			}

			// Try again every 5 seconds
			std::cout << "Trying again in 5 seconds" << std::endl;
			sleep(5);
		}

		while (!terminate)
		{
			// We are connected, perform our operations
			std::cout << "Please input your data (done to exit): ";
			std::cout.flush();

			// Get the data
			data_str.clear();
			std::getline(std::cin, data_str);
			data = ByteArray(data_str);
			expectedLength = data_str.size();

			// Must have data to send
			if (expectedLength == 0)
			{
				std::cout << "Cannot send no data!" << std::endl;
				continue;
			}
			else if (data_str == "done")
			{
				std::cout << "Closing the client..." << std::endl;
				terminate = true;
				break;
			}

			// Both Write/Read return the number of bytes sent/received
			if (socket.Write(data) <= 0 || socket.Read(data) <= 0)
			{
				std::cout << "Server failed to respond. Closing client..." << std::endl;
				terminate = true;
			}
			// Otherwise it was successful
			else
			{
				data_str = data.ToString();
				std::cout << "Server Response: " << data_str << std::endl;
			}
		}

		return 0;
	}
};

int main(void)
{
	// Welcome the user and try to initialize the socket
	std::cout << "SE3313 Lab 4 Client" << std::endl;

	// Create our socket
	//Socket socket("127.0.0.1", 3000);
	Socket socket("35.162.177.130", 3000);
	bool terminate = false;

	// Scope to kill thread
	{
		// Thread to perform socket operations on
		ClientThread clientThread(socket, terminate);

		while(!terminate)
		{
			// This will wait for 'done' to shutdown the server
			sleep(1);
		}
		
		// Wait to make sure the thread is cleaned up
	}
	
	// Attempt to close the socket
	try
	{
		socket.Close();
	}
	catch (...)
	{
		// We don't care if this failed because the application is exiting anyways.
	}

	return 0;
}
*/
//*LAB 4 GITHUB original CODE*/
/*
#include "thread.h"
#include "socket.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace Sync;

// This thread handles the connection to the server
class ClientThread : public Thread
{
private:
	// Reference to our connected socket
	Socket& socket;

	// Data to send to server
	ByteArray data;
	std::string data_str;
public:
	ClientThread(Socket& socket)
	: socket(socket)
	{}

	~ClientThread()
	{}

	virtual long ThreadMain()
	{
		int result = socket.Open();
		std::cout << "Please input your data (done to exit): ";
		std::cout.flush();

		// Get the data
		std::getline(std::cin, data_str);
		data = ByteArray(data_str);

		// Write to the server
		socket.Write(data);

		// Get the response
		socket.Read(data);
		data_str = data.ToString();
		std::cout << "Server Response: " << data_str << std::endl;
		return 0;
	}
};

int main(void)
{
	// Welcome the user 
	std::cout << "SE3313 Lab 4 Client" << std::endl;

	// Create our socket
	Socket socket("127.0.0.1", 3000);
	ClientThread clientThread(socket);
	while(1)
	{
		sleep(1);
	}
	socket.Close();

	return 0;
}
*/