
#include "thread.h"
#include "socket.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace Sync;



int main(void){
	std::cout << "SE3313 Lab 4 Client" << std::endl;

		Socket socket("127.0.0.1", 3000);
	
		ByteArray data;
		try{

		int result = socket.Open();
		} catch(...){
			std::cout<< "\nThere was an error opening\n";
			return -1;
		}
	

	while(true){
		std::string data_str;
		std::cout << "Please input your data (done to exit): ";
		std::cout.flush();

		// Get the data
		std::getline(std::cin, data_str);
		if (data_str == "done")
	        { break;}

data = ByteArray(data_str);
    
		

       
		// Write to the server
		int WriteError=socket.Write(data);
		if (WriteError==-1){
			std::cout<<"\n Server Socket is closed.\n";
			return -1;

		}


		// Get the response
		int ReadError=socket.Read(data);
		if (ReadError==0){
			std::cout<<"\nServer Socket is closed\n";
			return -1;

		}
	
		data_str = data.ToString();
		std::cout << "Server Response: " << data_str << std::endl;
		
		
	    
	}
	

	socket.Close();
	return 0;
}