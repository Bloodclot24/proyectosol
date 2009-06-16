#ifndef __COMMON_THREADRECEPTOR_H__
#define __COMMON_THREADRECEPTOR_H__

#include "threads.h"
#include "socket.h"
#include "mensaje.h"
#include "../HTTP/HttpResponse.h"
#include "deque.h"

class ThreadReceptor:Thread{
     Socket *socket;  /* el socket asociado */
     
     bool corriendo;  /* indica si el thread esta activo */
     bool ocupado;    /* indica si hay pedidos en espera */
     bool http;       /* indica si se va arecibir informacion http */
     
     void* buffer;    /* buffer donde almaceno los datos */
     int tamanioBuffer;

     Mutex mutexCola;
     Deque<char> colaDeDatos;
     Mutex mutexPedido;
     Mutex mutexEstado;
     CVariable pedido;
     CVariable estado;
     HttpResponse *response;
     
public:
     ThreadReceptor(Socket *socket, bool http);
     
     void esperarRecepcion();

     HttpResponse* getResponse();

     void comenzar();

     bool isRunning();

     void finalizar(void);
     
     Deque<char>* getColaDeDatos();
		     
protected:
     void run();
};

#endif
