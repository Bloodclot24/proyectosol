#ifndef COMMON_THREADS_INCLUDED
#define COMMON_THREADS_INCLUDED

#include <pthread.h>
#include <iostream>

class Mutex {

    private:
    pthread_mutex_t mutex;
    Mutex(const Mutex& otherInstance);
    Mutex& operator=(const Mutex& otherInstance);

    friend class CVariable;
    
    public:
    Mutex(){ pthread_mutex_init(&(this->mutex), NULL); }
    void lock(){ pthread_mutex_lock(&(this->mutex)); }
    void unlock(){ pthread_mutex_unlock(&(this->mutex)); }
    virtual ~Mutex() { pthread_mutex_unlock(&(this->mutex)); }

};


class CVariable {
    
    private:
    pthread_cond_t condition;
    Mutex *mutex;
    
    public:
    CVariable(Mutex *mutex){
	this->mutex = mutex; 
	pthread_cond_init (&condition, NULL); 
    }

    Mutex* getMutex(){ return mutex; }
    void wait(){ pthread_cond_wait(&condition, &(mutex->mutex)); }
    void signal(){ pthread_cond_signal(&condition); }
    void broadcast(){ pthread_cond_broadcast(&condition); }
    ~CVariable(){pthread_cond_broadcast(&condition);}
};

class Lock {
    
    private:
    Mutex &mutex;
    
    public:
    Lock(Mutex &m):mutex(m){ mutex.lock(); }
    ~Lock(){ mutex.unlock(); }
};


class Thread {
private:
     bool corriendo;  /* indica si el thread esta activo */
     pthread_t thread;
     Thread(const Thread& otherInstance);
     Thread& operator=(const Thread& otherInstance);
     static void* start_routine(void* thisPointer){
	  Thread* threadPointer = (Thread*) thisPointer;
	  
	  threadPointer->run();
	  return NULL;
     }
     
protected:
     void stop(){ corriendo = false; }
     Thread(){ corriendo = false; }
     virtual void run() = 0;
     void finish(){ stop(); }
     
public:
     int start(){ 
	  corriendo=true;
	  return pthread_create(&(this->thread), NULL, Thread::start_routine, this); 
     }

     bool isRunning(){ return corriendo; }

     void join(){ 
	  corriendo = false;
	  pthread_join(this->thread, NULL); 
     }

     virtual ~Thread(){ stop(); join(); }
};

#endif //COMMON_THREADS_INCLUDED
