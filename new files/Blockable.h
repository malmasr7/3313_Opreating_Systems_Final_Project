#ifndef BLOCKABLE_H
#define BLOCKABLE_H
#include <vector>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdarg.h>

namespace Sync {
	
typedef int TerminationException;

class Blockable
{
protected:
    int fd;
public:
    Blockable(int f=0):fd(f){;}
    Blockable(Blockable const & b) : fd(dup(b.fd)){;}
    virtual ~Blockable(void){;}
    operator int(void)const {return fd;}
    void SetFD(int f){fd =f;}
    int GetFD(void) const {return fd;}
};
extern Blockable cinWatcher;

// Is a semaphore a subclass of Event, or is an Event a subclass of Semaphore?  If you believe
// Marshal Cline, the answer is neither.  They are both descendents of PipeUser.
// This entire class has no public members.  It is there only to be inherited from.
class PipeUser : public Blockable
{
private:
    int sender; //The receiver is the base class
protected:
    PipeUser(void);
    PipeUser(PipeUser const &);
    PipeUser & operator=(PipeUser const &);
    void Assign(PipeUser const &);
    ~PipeUser(void);
    void BlockForByte(void);
    void WriteByte(char c='P');
    char ConsumeByte(void);
};

class Event : public PipeUser
{
public:
    Event(void){;}
    ~Event(){;}
    Event (Event const &);
    Event & operator=(Event const &);
    void Trigger(void);
    void Wait(void);
    void Reset(void);
};

class ThreadSem : public PipeUser
{
public:
    ThreadSem(int initialState=0);
    ThreadSem (ThreadSem const &);
    ~ThreadSem(){;}
    ThreadSem & operator=(ThreadSem const &);
    void Wait(void);
    void Signal(void);
};

class FlexWait
{
public:
    static const int FOREVER; //==-1
    static const int POLL; // == 0
private:
    std::vector<Blockable*> v;
public:
    FlexWait(int n,...);
    Blockable * Wait(int timeout=-1);
};
};
#endif // BLOCKABLE_H
