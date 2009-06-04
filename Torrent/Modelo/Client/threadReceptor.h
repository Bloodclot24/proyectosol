#ifndef __COMMON_THREADRECEPTOR_H__
#define __COMMON_THREADRECEPTOR_H__

#include "threads.h"
#include "socket.h"
#include "mensaje.h"

class ThreadReceptor:Thread{
     Socket *socket;  /* el socket asociado */
     
     bool corriendo;  /* indica si el thread esta activo */
     bool ocupado;    /* indica si hay pedidos en espera */
     
     void* buffer;    /* buffer donde almaceno los datos */
     int tamanioBuffer;

     Mutex mutexCola;
     std::deque<Mensaje*> colaDeMensajes;

     Mutex mutexPedido;
     Mutex mutexEstado;
     CVariable pedido;
     CVariable estado;

     
public:

     /* Saca el primer mensaje de la cola de mensajes */
     Mensaje* recibirMensaje(void){
	  Mensaje* mensaje=NULL;
	  Lock lock(mutexCola);
	  if(colaDeMensajes.size()>0){
	       mensaje = colaDeMensajes.front();
	       colaDeMensajes.pop_front();
	  }
	  return mensaje;
     }
     
     /* Si hay alguna operacion pendiante, espera a que termine */
     void esperarRecepcion(){
	  mutexCola.lock();
	  if(colaDeMensajes.size()>0){
	       mutexCola.unlock();
	  }
	  else{
	       mutexPedido.lock();
	       mutexCola.unlock();
	       pedido.wait();
	       mutexPedido.unlock();
	  }
     }
     
     
     /*
      * Crea un nuevo thread para la recepcion de datos mediante un
      * socket.
      */
     ThreadReceptor(Socket *socket):pedido(&mutexPedido),estado(&mutexEstado){
	  this->socket = socket;
	  corriendo = false;
	  ocupado = false;
	  buffer = NULL;
	  tamanioBuffer = 0;
     }

     /* Activa el thread */
     void comenzar(){
	  corriendo = true;
	  start();
     }
     
     /* finaliza el thread, cierra el socket y libera cualquier
      * mensaje que puediera quedar en la cola */
     void finalizar(void){
	  if(corriendo)
	       corriendo = false;
	  socket->setTimeout(0,1);
	  socket->cerrar();
	  mutexCola.lock();
	  while(!colaDeMensajes.empty()){
	       Mensaje *mensaje = colaDeMensajes.front();
	       colaDeMensajes.pop_front();
	       delete mensaje;
	  }
	  mutexCola.unlock();
     }
     
protected:

     /* Rutina principal del thread */
     void run(){
	  while(corriendo){
	       Mensaje* mensaje= new Mensaje;
/* 	       char tipo[LONGITUD_TIPO_MENSAJE]; */
/* 	       socket->recibir(&tipo, LONGITUD_TIPO_MENSAJE); */
/* 	       mensaje->setTipo(tipo); */
/* 	       socket->recibir(&(mensaje->tamanio), sizeof(mensaje->tamanio)); */
/* 	       mensaje->datos = new char[mensaje->tamanio+1]; */
/* 	       socket->recibir(mensaje->datos, mensaje->tamanio); */
/* 	       mensaje->datos[mensaje->tamanio]=0; */
	       mutexCola.lock();
	       colaDeMensajes.push_back(mensaje);

	       mutexPedido.lock();
	       pedido.signal();
	       mutexPedido.unlock();
	       mutexCola.unlock();
	  }
     }

};

#endif
