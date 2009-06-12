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
     sleep(10000);
     receptor.comenzar();
     emisor.comenzar();

     Mensaje *mensaje = new Mensaje;
     std::string aux = proto.handshake("BitTorrent protocol", hash, CLIENT_ID);

     mensaje->copiarDatos(aux.c_str(), aux.length());

     emisor.enviarMensaje(mensaje);

     Message *respuesta;

     while(corriendo){

////////////	  mensaje = receptor.recibirMensaje();

	  respuesta = proto.decode(mensaje->getDatos());
	  
	  switch(respuesta->id){

	  case CHOKE:
	       break;
	  case UNCHOKE:
	       break;
	  case INTERESTED:
	       break;
	  case NOT_INTERESTED:
	       break;
	  case HAVE:
	       break;
	  case BITFIELD:
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
}
