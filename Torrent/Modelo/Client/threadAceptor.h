#ifndef THREADACEPTOR_H_
#define THREADACEPTOR_H_

class  ThreadAceptor;

#include "socket.h"
#include "threads.h"
#include "peer.h"

class ThreadAceptor:Thread{
private:
     Socket *socket;
     
public:
     ThreadAceptor(Socket *socket);

     void comenzar();

     void finalizar(void);

     void run();

     ~ThreadAceptor();

};
   
#endif /*THREADACEPTOR_H_*/
