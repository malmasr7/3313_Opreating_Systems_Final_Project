#ifndef THREAD_H
#define THREAD_H

#include <stdlib.h>
#include <time.h>
#include <list>
#include <iostream>
#include <string>
#include <thread>
#include "Blockable.h"
void ThreadFunction(void * me);
class Thread
{
    friend void ThreadFunction(void * me);
private:
	std::thread theThread;
    int exitTimeout;

protected:
    Sync::Event terminationEvent;

private:
    Thread(Thread const &){}
    Thread & operator=(Thread const &){}
public:
    Thread(int exitTimeout = 1000);
    virtual ~Thread();

    // Override this function in your threads
    virtual long ThreadMain(void) = 0;
};

#endif // THREAD_H
