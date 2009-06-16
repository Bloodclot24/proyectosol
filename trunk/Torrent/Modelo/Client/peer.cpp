#include "peer.h"
#include "threadReceptor.h"
#include "threadEmisor.h"
#include "client.h"
#include "mensaje.h"

/****************************************************************************/
Peer::Peer(const std::string& host, int puerto , Torrent* torrent):	\
     socket(host, puerto),emisor(&socket),receptor(&socket, false){

     port = puerto;
     name = host;
     am_choking = 1;
     am_interested = 0;
     peer_choking = 1;
     peer_interested = 0;
     corriendo = false;
     this->torrent = torrent;
     this->bitField = new BitField(ceil(torrent->getBitField()->getLength()));
     conectado = false;
}

/****************************************************************************/
const std::string& Peer::getName(){
     return name;
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
void Peer::setInterested(bool state){
     Mensaje *mensaje = new Mensaje();
     ProtocoloBitTorrent proto;
     std::string msg;
     if(state)
	  msg = proto.interested();
     else msg = proto.not_interested();
     mensaje->copiarDatos(msg.c_str(), msg.length());
     emisor.enviarMensaje(mensaje);
     am_interested = state;
}

/****************************************************************************/
bool Peer::getInterested(){
     return am_interested;
}

/****************************************************************************/
void Peer::setChoke(bool state){
     Mensaje *mensaje = new Mensaje();
     ProtocoloBitTorrent proto;
     std::string msg;
     if(state)
	  msg = proto.choke();
     else msg = proto.unchoke();
     mensaje->copiarDatos(msg.c_str(), msg.length());
     emisor.enviarMensaje(mensaje);
     am_choking = state;
}

/****************************************************************************/
void Peer::have(uint32_t index){
     Mensaje *mensaje = new Mensaje();
     ProtocoloBitTorrent proto;
     std::string msg;
     msg = proto.have(index);
     mensaje->copiarDatos(msg.c_str(), msg.length());
     emisor.enviarMensaje(mensaje);
}

/****************************************************************************/
bool Peer::getChoke(){
     return am_choking;
}

/****************************************************************************/
void Peer::sendRequest(uint32_t index, uint32_t offset,		\
		       uint32_t size=REQUEST_SIZE_DEFAULT){
     
     Mensaje *mensaje = new Mensaje();
     ProtocoloBitTorrent proto;
     std::string msg = proto.request(index, offset, size);
     mensaje->copiarDatos(msg.c_str(), msg.length());
     emisor.enviarMensaje(mensaje);
     std::cout << "Request de: " << index << " tamaño: " << size << " offset: " << offset << std::endl;
}

/****************************************************************************/
bool Peer::havePiece(uint32_t index){
     return bitField->getField(index);
}

/****************************************************************************/
void Peer::run(){


     std::cout << "LLEGUE AL PEER\n";
     ProtocoloBitTorrent proto;

     //pongo un timeout de 30 seg. para la conexion al peer.
     socket.setTimeout(20,0);
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

     Mensaje *mensaje = new Mensaje();
     std::string aux = proto.handshake("BitTorrent protocol", hash, CLIENT_ID);

     mensaje->copiarDatos(aux.c_str(), aux.length());

     emisor.enviarMensaje(mensaje);

     mensaje = new Mensaje();
     aux = proto.bitfield(torrent->getBitField()->getBytesLength());
     mensaje->copiarDatos(aux.c_str(), aux.length());
     //emisor.enviarMensaje(mensaje);

     mensaje->copiarDatos(torrent->getBitField()->getData(), torrent->getBitField()->getBytesLength());
     //emisor.enviarMensaje(mensaje);
     

     Deque<char> *datos = receptor.getColaDeDatos();
     for(int i = 0; i <49+19;i++)
	  datos->popFront();

     conectado = true;

     /* Aviso al torrent que me pude conectar */
     torrent->peerConected(this);

     while(corriendo){

	  Message *respuesta = proto.decode(*datos);
	  
	  switch(respuesta->id){
	       
	  case CHOKE:
	       peer_choking = 1;
	       torrent->peerChoked(this);
	       break;
	  case UNCHOKE:
	       peer_choking = 0;
	       torrent->peerUnchoked(this);
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
		    for(int i=0;i < respuesta->length; i++)
			 bitField->setBlock(datos->popFront(),i);
	       }
	       else{
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
		    while(bloque.length() < respuesta->length){
			 bloque += datos->popFront();
		    }
		    std::cout << "Recibo el piece" << respuesta->index << " de tamanio " << respuesta->length << std::endl;
		    TorrentFile* file = torrent->obtenerArchivo(respuesta->index);
		    if(file != NULL){
			 std::cout << "Se escribe en el offset: " << respuesta->begin+torrent->obtenerByteOffset(respuesta->index) << \
			      ". Que corresponde al indice: " << respuesta->index << std::endl;

			 uint64_t restante = respuesta->length;

			 const char *datos = bloque.c_str();
			 uint64_t comienzo = respuesta->begin+torrent->obtenerByteOffset(respuesta->index);

			 while(restante > 0){
			      if(comienzo+restante <= file->getSize()){
				   // Cae toda la parte dentro del archivo
				   file->getManager()->agregarPieza(datos, comienzo, restante);
				   restante = 0;
			      }
			      else{
				   // Cae una parte adentro y otra parte en el siguiente archivo
				   file->getManager()->agregarPieza(datos, comienzo, file->getSize()-comienzo);
				   datos += file->getSize()-comienzo;
				   restante -= file->getSize()-comienzo;
				   // obtengo el proximo archivo
				   file = torrent->obtenerArchivo(++respuesta->index);
				   comienzo = 0;
			      }
			 }
		    }
		    else{
			 std::cerr << "ERROR: <--------------------- NO EXISTE EL ARCHIVO??????\n";
		    }
		    if(peer_choking == 0)
			 torrent->peerUnchoked(this);
		    
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
