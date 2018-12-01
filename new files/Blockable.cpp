#include "Blockable.h"
#include <algorithm>
#include "errno.h"
#include <iostream>
#include "stdio.h"

namespace Sync{
	
Blockable cinWatcher(0);

PipeUser::PipeUser(void)
{
    int pipesFD[2];
    pipe(pipesFD);
    SetFD(pipesFD[0]);
    sender = pipesFD[1];
}

PipeUser::PipeUser (PipeUser const &p)
    : Blockable(dup(p.fd)), sender(dup(p.sender))
{
    ;
}

void PipeUser::Assign(PipeUser const & p)
{
    close(sender);
    close(GetFD());
    SetFD(p.GetFD());
    sender = dup(p.sender);
}

PipeUser & PipeUser::operator = (PipeUser const & p)
{
    Assign(p);
    return *this;
}

PipeUser::~PipeUser ()
{
    close(sender);
    close(GetFD());
}

// Wait is designed to be non-intrusive. Returns when character is available
void PipeUser::BlockForByte(void)
{
    FlexWait f(1,this);
    f.Wait();
}
char PipeUser::ConsumeByte(void)
{
    char c;
    read(GetFD(),&c,1);
    return c;
}
void PipeUser::WriteByte(char c)
{
    write(sender,&c,1);
}

Event::Event(Event const & e)
    :PipeUser(e)
{
    ;
}

Event & Event::operator = (Event const & e)
{
    Assign(e);
    return *this;
}

void Event::Trigger(void)
{
    PipeUser::WriteByte('E');
}

void Event::Wait(void)
{
    PipeUser::BlockForByte();
}

void Event::Reset(void)
{
    PipeUser::ConsumeByte();
}

ThreadSem::ThreadSem(int initialState)
    :PipeUser()
{
    for (int i=0;i<initialState;i++)
        Signal();
}

ThreadSem::ThreadSem(ThreadSem const & e)
    :PipeUser(e)
{
    ;
}

ThreadSem & ThreadSem::operator = (ThreadSem const & e)
{
    Assign(e);
    return *this;
}

void ThreadSem::Wait(void)
{
    PipeUser::BlockForByte();
    PipeUser::ConsumeByte();
}

void ThreadSem::Signal(void)
{
    PipeUser::WriteByte();
}

FlexWait::FlexWait(int n,... )
{
    va_list vl;
    va_start(vl,n);
    for (int i=0;i<n;i++)
    {
        v.push_back(va_arg(vl,Blockable*));
    }
}

// Returns a pointer to a blockable object or throws TerminationException
// Return of 0 means the wait timed out.  Time out parameter of zero means
// never time out.
void ShowFD_Set (fd_set & theSet)
{
    int total = 1024 / (8 * (int) sizeof (__fd_mask));
    for (int i=0;i<total; i++)
    {
        if (theSet.fds_bits[i])
            std::cout << i << " contains something " << std::hex << theSet.fds_bits[i] << std::endl;
    }

}
void ShowParams(std::vector<Blockable *> const & v, int maxArg)
{
    for (int i=0;i<v.size();i++)
    {
        std::cout << " fd:" << v[i]->GetFD();
    }
    std::cout << " maxarg: " << maxArg << std::endl;
}

//
const int FlexWait::FOREVER = -1;
Blockable * FlexWait::Wait(int timeout)
{
    timeval tv;
    int maxFD = 0;
    int fd = 0;
    tv.tv_sec = timeout/1000;
    tv.tv_usec = (timeout%1000)*1000;
    timeval * pTimeout = &tv;

    if (timeout == -1)
        pTimeout = 0;

    fd_set theSet;
    FD_ZERO(&theSet);
    for (int i=0;i<v.size();i++)
    {
        fd = v[i]->GetFD();
        maxFD = std::max(maxFD,fd);
        FD_SET(fd,&theSet);
    }
    int maxFDArg = maxFD+1;
    //ShowParams(v,maxFDArg);
    int selectionFD = select(maxFDArg, &theSet, NULL, NULL, pTimeout);

    if (selectionFD < 0)
    {
        perror("select");
        ShowParams(v,maxFDArg);
        throw std::string("Unexpected error in synchronization object");
    }

    if (selectionFD == 0)
        return 0;

    for (int i=0;i<v.size();i++)
    {
        int fd = v[i]->GetFD();
        if (FD_ISSET(fd,&theSet))
            return v[i];
    }
    throw std::string("Unknown error in synchronization object");
}
};