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
     Deque<char*> colaDeDatos;
     HttpResponse *response: /* respuesta http */

     Mutex mutexPedido;
     Mutex mutexEstado;
     CVariable pedido;
     CVariable estado;

     
public:
};

#endif
