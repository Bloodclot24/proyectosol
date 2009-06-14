#include "peer.h"
#include "threadReceptor.h"
#include "threadEmisor.h"
#include "client.h"
#include "mensaje.h"

/****************************************************************************/
Peer::Peer(const std::string& host, int puerto , Torrent* torrent):	\
     socket(host, puerto),emisor(&socket),receptor(&socket, false){

     am_choking = 1;
     am_interested = 0;
     peer_choking = 1;
     peer_interested = 0;
     corriendo = false;
     this->torrent = torrent;
     this->bitField = new BitField(ceil(torrent->getBitField()->getLength()));
     std::cout << "--------------------__>>>>>>> LONGITUD DEL BITFIELD: " << ceil(torrent->getBitField()->getLength()) << "\n";
     conectado = false;
}

/****************************************************************************/
void Peer::start(std::string hash){
     this->hash = hash;
     corriendo = true;
     Thread::start();
}

/****************************************************************************/
const BitField* Peer::getBitField(){
     return bitField;
}

/****************************************************************************/
ThreadEmisor* Peer::getEmisor(){
     return &emisor;
}

/****************************************************************************/
void Peer::run(){

     ProtocoloBitTorrent proto;

     //pongo un timeout de 30 seg. para la conexion al peer.
//     socket.setTimeout(10,0);
     socket.conectar();
     if(!socket.esValido()){
	  corriendo = false;
	  std::cout << "ERROR DEL SOCKET: " << socket.obtenerError() << std::endl;
	  torrent->eliminarPeer(this);
	  return;
     }
     

     std::cout << "CONECTADO EXITOSAMENTE ------------<<<<" << std::endl;

     receptor.comenzar();
     emisor.comenzar();

     Mensaje *mensaje = new Mensaje;
     std::string aux = proto.handshake("BitTorrent protocol", hash, CLIENT_ID);

     mensaje->copiarDatos(aux.c_str(), aux.length());

     emisor.enviarMensaje(mensaje);

     Deque<char> *datos = receptor.getColaDeDatos();
     for(int i = 0; i <49+19;i++)
	  datos->popFront();

     conectado = true;

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
	       if(respuesta->length == ceil(torrent->getBitField()->getLength()/8)+1){
		    std::cout << "Longitud del bitfield OK\n";
		    for(int i=0;i < respuesta->length; i++)
			 bitField->setBlock(datos->popFront(),i);
	       }
	       else{
		    std::cout << "Se esperaba una longitud de : " << ceil(torrent->getBitField()->getLength()/8) << "\n";
		    //Cerrar conexion y salir
		    corriendo = false;
	       }
	       break;
	  case REQUEST:
	       if(torrent->getBitField()->getField(respuesta->index) ==1){
		    //OK, me piden una pieza que tengo
		    //Genero el mensaje del protocolo
		    std::string aux = proto.piece(respuesta->index, respuesta->begin, respuesta->length);
		    mensaje->copiarDatos(aux.c_str(), aux.length());
		    emisor.enviarMensaje(mensaje);
		    
		    TorrentFile* file = torrent->obtenerArchivo(respuesta->index);
		    if(file != NULL){
			 char *bloque = new char[respuesta->length];
			 file->getManager()->obtenerPieza(bloque, respuesta->begin, respuesta->length);
			 mensaje->asignarDatos(bloque, respuesta->length);
			 emisor.enviarMensaje(mensaje);
		    }
	       }
	       break;
	  case PIECE:
	       if(torrent->getBitField()->getField(respuesta->index) == 0){
		    //OK, me envian datos que no tenia. TODO:
		    //verificar que el offset se corresponda con datos
		    //que no tenemos
		    std::string bloque;
		    while(bloque.length() < respuesta->length)
			 bloque += datos->popFront();
		    TorrentFile* file = torrent->obtenerArchivo(respuesta->index);
		    if(file != NULL)
			 file->getManager()->agregarPieza(bloque.c_str(), respuesta->begin, respuesta->length);
		    
	       }
	       else{
		    corriendo = false;
	       }
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

     conectado = false;
     receptor.finalizar();
     emisor.finalizar();
     torrent->eliminarPeer(this);
}
