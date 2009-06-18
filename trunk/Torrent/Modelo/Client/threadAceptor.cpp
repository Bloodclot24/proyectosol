#include "threadAceptor.h"

ThreadAceptor::ThreadAceptor(Socket *socket){
     this->socket = socket;
     corriendo = false;
}

void ThreadAceptor::comenzar(){
     corriendo = true;
     start();
}

void ThreadAceptor::finalizar(void){
     corriendo = false;
     socket->setTimeout(0,1);
     socket->cerrar();
}

void ThreadAceptor::run(){
	while(corriendo){
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
