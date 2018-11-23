#include "thread.h"
#include "socketserver.h"

#include "socketThread.h"
#include "socket.h"

class SocketThread : public Thread
{

public:

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
                ServerThread::socketThreadPairs[placement].getPartner(socket).Write(data);       //in pair, get socket's partner, and write the data message to this partner
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