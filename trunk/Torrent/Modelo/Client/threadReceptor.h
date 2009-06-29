#ifndef __COMMON_THREADRECEPTOR_H__
#define __COMMON_THREADRECEPTOR_H__

#include "threads.h"
#include "socket.h"
#include "mensaje.h"
#include "../HTTP/HttpResponse.h"
#include "deque.h"

/** 
 * Thread que se utiliza para recibir datos mediante un Socket.
 */
class ThreadReceptor:public Thread{
     Socket *socket;		/**< El socket asociado */
     
     bool ocupado;		/**< Indica si hay pedidos en espera  */
     bool http;			/**< Indica si se va a recibir
				 * informacion http */
     
     Deque<char> colaDeDatos;	/**< Cola donde se almacenan los datos
				 * recibidos */

     HttpResponse *response;	/**< Respuesta HTTP recibida. */

     Mutex mutexHttp;		/**< Mutex asociado a condHttp */
     CVariable condHttp;	/**< CVariable para HTTP */
     
public:
     /** 
      * Crea un nuevo ThreadReceptor con un Socket dado.
      * 
      * @param socket El socket mediante el cual se recibiran los
      * datos.
      *
      * @param http Flag indicando si la conexion debe ser
      * interpretada como HTTP o no.
      */
     ThreadReceptor(Socket *socket, bool http);

     /** 
      * Devuelve la respuesta HTTP recibida.
      */
     HttpResponse* getResponse();

     /** 
      * Comienza el Thread.
      */
     void comenzar();
     
     /** 
      * Finaliza el thread.
      */
     void finish(void);
     
     /** 
      * Devuelve la cola de datos donde se van almacenando los
      * mensajes recibidos mediante el Socket.
      * 
      * @return La cola de datos.
      *
      * @see Deque
      */
     Deque<char>* getColaDeDatos();

     /** 
      * Elimina el Thread.
      */
     ~ThreadReceptor(){
	  std::cerr << "chau receptorrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr" << &colaDeDatos <<"\n";
	  finish();
     };
		     
protected:
     /** 
      * Metodo principal del Thread.
      * @see Thread
      */
     void run();
};

#endif
