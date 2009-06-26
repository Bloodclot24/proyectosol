#ifndef __COMMON_THREADEMISOR_H__
#define __COMMON_THREADEMISOR_H__

#include "threads.h"
#include "socket.h"
#include "mensaje.h"
#include "deque.h"

class ThreadEmisor:public Thread{
     Socket *socket;
     
     bool ocupado;
     
     Mutex mutexCola;
     Deque<Mensaje*> colaDeEspera;
     

public:
     /* Envia un mensaje a treves del socket. La funcion es no
      * bloqueante. Agrega el mensaje a una cola de datos y espera a
      * poder enviarlo. */
     bool enviarMensaje(Mensaje* mensaje){
	  colaDeEspera.push(mensaje);
	  return true;
     }
     
     /* Crea un nuevo thread asociado a un socket para emitir datos */
     ThreadEmisor(Socket *socket){
	  this->socket = socket;
	  ocupado = false;
     }

     /* Comienza el thread */
     void comenzar(void){
	  start();
     }
     
     /* Finaliza el thread, cierra el socket y borra cualquier mensaje
      * que pudiera quedar en la cola */
     void finalizar(void){
	  if(isRunning()){
	       join();
	       socket->setTimeout(0,1);
	       socket->cerrar();
	       while(!colaDeEspera.empty()){
		    Mensaje *mensaje = colaDeEspera.popFront();
		    delete mensaje;
	       }
	  }
     }

private:     

     /* Funcion principal del thread */
     void run(){
	  while(isRunning()){
	       Mensaje *mensaje = colaDeEspera.popFront();
	       socket->enviar(mensaje->getDatos(), mensaje->getTamanio());
	       //delete mensaje;
	  }
     }
};

#endif
