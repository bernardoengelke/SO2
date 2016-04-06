// EPOS Synchronizer Abstractions Common Package

#ifndef __synchronizer_h
#define __synchronizer_h

#include <cpu.h>
#include <thread.h>

__BEGIN_SYS

class Synchronizer_Common {
private:
    Thread::Queue queue;

protected:
    Synchronizer_Common() {}

    // Atomic operations
    bool tsl(volatile bool & lock) { return CPU::tsl(lock); }
    int finc(volatile int & number) { return CPU::finc(number); }
    int fdec(volatile int & number) { return CPU::fdec(number); }

    // Thread operations
    void begin_atomic() { Thread::lock(); }
    void end_atomic() { Thread::unlock(); }

    void sleep() { 
        //Thread::yield(); 
        begin_atomic();
        Thread* t = Thread::self();
        t->state = Thread::WAITING;
        Thread::_suspended.insert(&t->_link);

        // if(!Thread::_ready.empty()){
        //     Thread* nextThread = Thread::_ready.remove()->object();
        //     nextThread
        // }
        Thread::yield();
        end_atomic();
    } // implicit unlock()
    void wakeup() { 
        begin_atomic();
        if(!Thread::_suspended.empty()){
            Thread* t = Thread::_suspended.remove()->object();
            t->state = Thread::READY;
            Thread::_ready.insert(&t->_link);
        }
        end_atomic(); 
    }
    void wakeup_all() { end_atomic(); }
};

__END_SYS

#endif

