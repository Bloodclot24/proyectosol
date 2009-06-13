#include "peer.h"
#include "threadReceptor.h"
#include "threadEmisor.h"
#include "client.h"
#include "mensaje.h"

Peer::Peer(const std::string& host, int puerto , Torrent* torrent):	\
     socket(host, puerto),emisor(&socket),receptor(&socket, false){

     am_choking = 1;
     am_interested = 0;
     peer_choking = 1;
     peer_interested = 0;
     corriendo = false;
     this->torrent = torrent;
     this->bitField = new BitField(torrent->getBitField()->getLength());
}

void Peer::start(std::string hash){
     this->hash = hash;
     corriendo = true;
     Thread::start();
}

void Peer::run(){

     ProtocoloBitTorrent proto;

     socket.conectar();
     if(!socket.esValido()){
	  corriendo = false;
	  std::cout << socket.obtenerError() << std::endl;
	  return;
     }

     std::cout << "CONECTADO EXITOSAMENTE -----------------------------<<<<" << std::endl;

     receptor.comenzar();
     emisor.comenzar();

     Mensaje *mensaje = new Mensaje;
     std::string aux = proto.handshake("BitTorrent protocol", hash, CLIENT_ID);

     mensaje->copiarDatos(aux.c_str(), aux.length());

     emisor.enviarMensaje(mensaje);

     Deque<char> *datos = receptor.getColaDeDatos();
     for(int i = 0; i <49+19;i++)
	  datos->popFront();

     while(corriendo){

	  Message *respuesta = proto.decode(*datos);
	  
	  switch(respuesta->id){
	       
	  case CHOKE:
	       peer_choking = 1;
	       break;
	  case UNCHOKE:
	       peer_choking = 0;
	       break;
	  case INTERESTED:
	       peer_interested = 1;
	       break;
	  case NOT_INTERESTED:
	       peer_interested = 0;
	       break;
	  case HAVE:
	       bitField->setField(respuesta->index, 1);
	       break;
	  case BITFIELD:
	       if(respuesta->length == torrent->getBitField()->getLength()/8)
		    for(int i=0;i < respuesta->length; i++)
			 bitField->setBlock(datos->popFront(),i);
	       else
		    //Cerrar conexion y salir
		    corriendo = false;
	       break;
	  case REQUEST:
	       
	       break;
	  case PIECE:
	       
	       break;
	  case CANCEL:
	       
	       break;
	  case PORT:
	       
	       break;
	  case KEEP_ALIVE:
	       
	       break;

	  default:
	       break;
	  }
     }

     receptor.finalizar();
     emisor.finalizar();
}
