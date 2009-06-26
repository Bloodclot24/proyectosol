#include "threadAceptor.h"

ThreadAceptor::ThreadAceptor(Socket *socket){
     this->socket = socket;
}

void ThreadAceptor::comenzar(){
     start();
}

void ThreadAceptor::finish(void){
     if(isRunning()){
	  Thread::finish();
	  socket->setTimeout(0,1);
	  socket->cerrar();
     }
}

void ThreadAceptor::run(){
     while(isRunning()){
	  Socket *sck=NULL;
	  sck = socket->aceptar();  //Acepto el peer
	  if(sck){
	       Peer *peer = new Peer(sck); //creo el peer
	       peer->start(""); //comienza el procesamiento
	  }
     }
}

ThreadAceptor::~ThreadAceptor(){
     delete socket;
}
