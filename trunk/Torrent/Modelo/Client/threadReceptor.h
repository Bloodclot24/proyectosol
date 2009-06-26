#ifndef __COMMON_THREADRECEPTOR_H__
#define __COMMON_THREADRECEPTOR_H__

#include "threads.h"
#include "socket.h"
#include "mensaje.h"
#include "../HTTP/HttpResponse.h"
#include "deque.h"

class ThreadReceptor:public Thread{
     Socket *socket;  /* el socket asociado */
     
     bool ocupado;    /* indica si hay pedidos en espera */
     bool http;       /* indica si se va arecibir informacion http */
     
     void* buffer;    /* buffer donde almaceno los datos */
     int tamanioBuffer;

     Deque<char> colaDeDatos;

     HttpResponse *response;

     Mutex mutexHttp;
     CVariable condHttp;
     
public:
     ThreadReceptor(Socket *socket, bool http);

     HttpResponse* getResponse();

     void comenzar();

     void finish(void);
     
     Deque<char>* getColaDeDatos();
		     
protected:
     void run();
};

#endif
