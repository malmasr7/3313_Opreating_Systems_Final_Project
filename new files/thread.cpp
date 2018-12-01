#include "thread.h"
#include "unistd.h"
#include <iostream>

Thread::Thread(int exitT/*=1000*/)
: exitTimeout(exitT),
 theThread(ThreadFunction,this)
{
	;
}

Thread::~Thread()
{
    Sync::FlexWait theEnd(1,&terminationEvent);
    if (theEnd.Wait(exitTimeout))
    {
        theThread.join();
    }
    else
        std::cout << "Th: Unable to properly terminate thread" << std::endl;
}

void ThreadFunction(void * me)
{
    // The parameter was the "this" pointer, passed in Thread::Start.  It can
    // be safely cast back to Thread*
    Thread * theThread = (Thread*)me;

    try
    {
        theThread->ThreadMain(); // Call a derived ThreadMain.
    }
    catch(Sync::TerminationException)
    {
        ;
    }
    theThread->terminationEvent.Trigger();
}
