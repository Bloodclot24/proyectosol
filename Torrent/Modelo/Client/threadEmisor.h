#ifndef __COMMON_THREADEMISOR_H__
#define __COMMON_THREADEMISOR_H__

#include "threads.h"
#include "socket.h"
#include "mensaje.h"
#include "deque.h"

/** 
 * Thread utilizado para enviar datos atraves de un Socket.
 */
class ThreadEmisor:public Thread{
     Socket *socket;		/**< Socket a utilizar. */
     
     bool ocupado;		/**< Indica si esta o no ocupado el
				 * Thread */
     
     Deque<Mensaje*> colaDeEspera; /**< Cola de datos donde se guardan
				    * los mensajes a enviar. */
     

public:

     /** 
      * Crea un nuevo thread asociado a un socket para emitir datos.
      * 
      * @param socket El socket.
      */
     ThreadEmisor(Socket *socket){
	  this->socket = socket;
	  ocupado = false;
     }

     /** 
      *  Envia un mensaje atraves del socket. La funcion es no
      *  bloqueante. Agrega el mensaje a una cola de datos y espera a
      *  poder enviarlo.
      * 
      * @param mensaje El Mensaje a enviar.
      * 
      * @return true si pudo poner el mensaje en la cola.
      *
      * @see Mensaje
      */
     bool enviarMensaje(Mensaje* mensaje){
	  colaDeEspera.push(mensaje);
	  return true;
     }

     /** 
      * Espera a que se terminen de enviar todos los mensajes
      * pendientes en la cola.
      */
     void esperarEmision(){
	  while(!colaDeEspera.empty());
     }
    

     /** 
      * Comienza el Thread.
      */
     void comenzar(void){
	  start();
     }
     
     /** 
      * Finaliza el thread, cierra el socket y borra cualquier mensaje
      * que pudiera quedar en la cola.
      * 
      */
     void finish(void){
	  if(isRunning()){
	       socket->setTimeout(0,1);
	       Thread::finish();
	       socket->cerrar();
	       while(!colaDeEspera.empty()){
		    Mensaje *mensaje = colaDeEspera.popFront();
		    //delete mensaje;
	       }
	  }
     }

private:     

     /** 
      * Funcion principal del thread.
      *
      * @see Threads
      */
     void run(){
	  colaDeEspera.hold();
	  while(isRunning()){
	       Mensaje *mensaje = colaDeEspera.popFront();
	       if(colaDeEspera.isValid())
		    socket->enviar(mensaje->getDatos(), mensaje->getTamanio());
	       else stop();
	       //delete mensaje;
	  }
	  colaDeEspera.release();
     }
public:
     
     /** 
      * Elimina el Thread.
      */
     ~ThreadEmisor(){};
};

#endif
