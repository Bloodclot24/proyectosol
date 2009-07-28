#include "threadAceptor.h"

ThreadAceptor::ThreadAceptor(Socket *socket, Client *cliente){
     this->socket = socket;
     this->cliente = cliente;
}

void ThreadAceptor::comenzar(){
     start();
}

void ThreadAceptor::finish(void){
     if(isRunning()){
	  socket->setTimeout(0,1);
	  Thread::finish();
	  socket->cerrar();
     }
}

void ThreadAceptor::run(){
     while(isRunning()){
	  Socket *sck=NULL;
	  sck = socket->aceptar();  //Acepto el peer
	  if(sck){
	       Peer *peer = new Peer(sck,cliente); //creo el peer
	       peer->start(""); //comienza el procesamiento
	  }
     }
}

ThreadAceptor::~ThreadAceptor(){
     delete socket;
}
