#ifndef __COMMON_THREADEMISOR_H__
#define __COMMON_THREADEMISOR_H__

#include "threads.h"
#include "socket.h"
#include "mensaje.h"
#include <deque>

class ThreadEmisor:Thread{
     Socket *socket;
     
     bool corriendo;
     bool ocupado;
     
     void* buffer;
     int tamanioBuffer;

     Mutex mutexCola;
     std::deque<Mensaje*> colaDeEspera;
     
     Mutex mutexPedido;
     Mutex mutexEstado;
     CVariable pedido;
     CVariable estado;

public:
     /* Envia un mensaje a treves del socket. La funcion es no
      * bloqueante. Agrega el mensaje a una cola de datos y espera a
      * poder enviarlo. */
     bool enviarMensaje(Mensaje* mensaje){
	  Lock lock1(mutexPedido);
	  mutexCola.lock();
	  colaDeEspera.push_back(mensaje);
	  mutexCola.unlock();
	  pedido.signal();
	  return true;
     }
     
     void esperarEnvio(){
     }
     
     /* Crea un nuevo thread asociado a un socket para emitir datos */
     ThreadEmisor(Socket *socket):pedido(&mutexPedido),estado(&mutexEstado){
	  this->socket = socket;
	  corriendo = false;
	  ocupado = false;
	  buffer = NULL;
	  tamanioBuffer = 0;
     }

     /* Comienza el thread */
     void comenzar(void){
	  corriendo = true;
	  start();
     }
     
     /* Finaliza el thread, cierra el socket y borra cualquier mensaje
      * que pudiera quedar en la cola */
     void finalizar(void){
	  if(corriendo)
	       corriendo = false;
	  socket->setTimeout(0,1);
	  socket->cerrar();
	  mutexCola.lock();
	  while(!colaDeEspera.empty()){
	       Mensaje *mensaje = colaDeEspera.front();
	       colaDeEspera.pop_front();
	       delete mensaje;
	  }
	  mutexCola.unlock();
     }

private:     

     /* Funcion principal del thread */
     void run(){
	  while(corriendo){
	       mutexCola.lock();
	       if(colaDeEspera.size() > 0){
		    Mensaje *mensaje = colaDeEspera.front();
		    colaDeEspera.pop_front();
		    mutexCola.unlock();
		    socket->enviar(mensaje->getDatos(), mensaje->getTamanio());
		    //delete mensaje;
	       }
	       else{
		    mutexPedido.lock();
		    mutexCola.unlock();
		    pedido.wait();
		    mutexPedido.unlock();
	       }
	  }
     }
};

#endif
