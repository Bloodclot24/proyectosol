#include "peer.h"
#include "threadReceptor.h"
#include "threadEmisor.h"
#include "client.h"
#include "mensaje.h"

/****************************************************************************/
Peer::Peer(const std::string& host, int puerto , Torrent* torrent){
     port = puerto;
     name = host;
     am_choking = 1;
     am_interested = 0;
     peer_choking = 1;
     peer_interested = 0;
     this->torrent = torrent;
     this->bitField = new BitField(ceil(torrent->getBitField()->getLength()));
     conectado = false;
     this->entrante = false;
     downloaded = 0;
     uploaded = 0;
     corrupted = 0;
     colaDatos = NULL;
}

/****************************************************************************/
Peer::Peer(Socket* socket){
     port = PORT_IN;
     name = "";
     am_choking = 1;
     am_interested = 0;
     peer_choking = 1;
     peer_interested = 0;
     this->socket = socket;
     conectado = false;
     this->torrent = NULL;
     this->bitField = NULL;
     this->entrante = true;
     downloaded = 0;
     uploaded = 0;
     corrupted = 0;
}

/****************************************************************************/
const std::string& Peer::getName(){
     return name;
}

/****************************************************************************/
void Peer::start(std::string hash){
     this->hash = hash;
     Thread::start();
}

/****************************************************************************/
const BitField* Peer::getBitField(){
     return bitField;
}

/****************************************************************************/
ThreadEmisor* Peer::getEmisor(){
     return emisor;
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
     emisor->enviarMensaje(mensaje);
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
     emisor->enviarMensaje(mensaje);
     am_choking = state;
}

/****************************************************************************/
void Peer::have(uint32_t index){
     Mensaje *mensaje = new Mensaje();
     ProtocoloBitTorrent proto;
     std::string msg;
     msg = proto.have(index);
     mensaje->copiarDatos(msg.c_str(), msg.length());
     emisor->enviarMensaje(mensaje);
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
     emisor->enviarMensaje(mensaje);
     std::cout << "Request de: " << index << " tama�o: " << size << " offset: " << offset << std::endl;
}
/****************************************************************************/
void Peer::sendRequest(DownloadSlot* ds){
     Mensaje *mensaje = new Mensaje();
     ProtocoloBitTorrent proto;
     std::string msg = proto.request(ds->getPieceIndex(), ds->getOffset(), ds->getLength());
     mensaje->copiarDatos(msg.c_str(), msg.length());
     requests.push(ds);
     emisor->enviarMensaje(mensaje);
     std::cout << "Request de: " <<  ds->getPieceIndex() << " tamao: " \
	       << ds->getOffset() << " offset: " << ds->getLength()	\
	       << std::endl;
}


/****************************************************************************/
bool Peer::havePiece(uint32_t index){
     return bitField->getField(index);
}

/****************************************************************************/
void Peer::run(){
     ProtocoloBitTorrent proto;
     Deque<char> *datos = NULL;
     Mensaje *mensaje = NULL;
     conectado = true;
     if(!this->entrante){
	  this->socket = new Socket(name,port);
	  this->emisor = new ThreadEmisor(this->socket);
	  this->receptor = new ThreadReceptor(this->socket,false);
     
	  socket->conectar();
	  if(!socket->esValido()){
	       stop();
	       std::cout << "ERROR DEL SOCKET: " << socket->obtenerError() << std::endl;
	       conectado = false;
	       torrent->eliminarPeer(this);
	       return;
	  }

	  std::cout << "CONECTADO EXITOSAMENTE ------------<<<<" << std::endl;

	  receptor->comenzar();
	  emisor->comenzar();

	  /* Envio un handshake */
	  mensaje = new Mensaje();
	  std::string aux = proto.handshake("BitTorrent protocol", hash, CLIENT_ID);
	  mensaje->copiarDatos(aux.c_str(), aux.length());
	  emisor->enviarMensaje(mensaje);

	  /* Envio un bitfield con las piezas que ya tenemos */
	  mensaje = new Mensaje();
	  aux = proto.bitfield(torrent->getBitField()->getBytesLength());
	  mensaje->copiarDatos(aux.c_str(), aux.length());
	  emisor->enviarMensaje(mensaje);
	  mensaje = new Mensaje();
	  mensaje->copiarDatos(torrent->getBitField()->getData(), torrent->getBitField()->getBytesLength());
	  emisor->enviarMensaje(mensaje);

	  /* Recibo el handshake del peer */
	  colaDatos = datos = receptor->getColaDeDatos();
	  if(datos->hold()){
	       for(int i = 0; i <49+19;i++)
		    datos->popFront();
	       conectado = true;
	  }
	  else
	       conectado = false;
     
     }else{
     	
	  this->emisor = new ThreadEmisor(this->socket);
	  this->receptor = new ThreadReceptor(this->socket,false);
	  receptor->comenzar();
	  emisor->comenzar();
	  char data;
	  std::string hashRecibido;
	  char pstrlen;
	  int contador = 0;
	  bool primerByte = true;

	  /* Recibo el handshake del peer */
	  colaDatos = datos = receptor->getColaDeDatos();
	  if(datos->hold()){
	       for(int i = 0; i <49+19;i++){
		    data = datos->popFront();
		    //Obtengo la longitud de pstr
		    if(primerByte){
			 primerByte = false;
			 pstrlen = data;
		    }
		    //obtengo el hash del handshake
		    if ((i > pstrlen + 8) && (contador < 20)){
			 hashRecibido += data;
			 contador++;
		    }
	       }
	  }
	  //asigno el hash obtenido al peer
	  this->hash.clear();
	  this->hash = hashRecibido;
     
	  /* Envio un handshake */
	  mensaje = new Mensaje();
	  std::string aux = proto.handshake("BitTorrent protocol", hash, CLIENT_ID);
	  mensaje->copiarDatos(aux.c_str(), aux.length());
	  emisor->enviarMensaje(mensaje);

	  /* Envio un bitfield con las piezas que ya tenemos */
	  mensaje = new Mensaje();
	  aux = proto.bitfield(torrent->getBitField()->getBytesLength());
	  mensaje->copiarDatos(aux.c_str(), aux.length());
	  emisor->enviarMensaje(mensaje);
	  mensaje = new Mensaje();
	  mensaje->copiarDatos(torrent->getBitField()->getData(), torrent->getBitField()->getBytesLength());
	  emisor->enviarMensaje(mensaje);
     }


     if(conectado)
	  /* Aviso al torrent que me pude conectar */
	  torrent->peerConected(this);
     else
	  stop();

     while(isRunning() && receptor->isRunning()){
	  Message *respuesta = proto.decode(*datos);
	  if(!datos->isValid()){
	       stop();
	  }
	  else{
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
		    if(respuesta->length == torrent->getBitField()->getBytesLength()){
			 uint32_t i;
			 for(i=0;i < respuesta->length && datos->isValid(); i++)
			      bitField->setBlock(datos->popFront(),i);     
			 if(i < respuesta->length)
			    stop();
		    }
		    else{
			 //Cerrar conexion y salir
			 std::cout << "Bitfield de longitud erronea recibido \n";
			 stop();
		    }
		    break;
	       case REQUEST:
		    if(torrent->getBitField()->getField(respuesta->index) ==1){
			 //OK, me piden una pieza que tengo
			 //Genero el mensaje del protocolo
			 std::string aux = proto.piece(respuesta->index, respuesta->begin, respuesta->length);
			 mensaje = new Mensaje();
			 mensaje->copiarDatos(aux.c_str(), aux.length());
			 emisor->enviarMensaje(mensaje);

			 std::cout << "Me piden el piece" << respuesta->index << " offset "<< respuesta->begin << " de tamanio " << respuesta->length << std::endl;
		    
			 char *bloque = new char[respuesta->length];
			 torrent->readData(bloque, respuesta->index, respuesta->begin, respuesta->length);
			 mensaje = new Mensaje();
			 mensaje->asignarDatos(bloque, respuesta->length);
			 emisor->enviarMensaje(mensaje);
			 uploaded += respuesta->length;
		    }
		    break;
	       case PIECE:
	       {
		    //OK, me envian datos. 
		    std::string bloque;
		    while(bloque.length() < respuesta->length && datos->isValid()){
			 bloque += datos->popFront();
		    }
		    std::cout << "Recibo el piece" << respuesta->index << " de tamanio " << respuesta->length << std::endl;
		    std::cout << "Se escribe en el offset: " << respuesta->begin+torrent->obtenerByteOffset(respuesta->index) << \
			 ". Que corresponde al indice: " << respuesta->index << std::endl;
		    torrent->writeData(bloque.c_str(), respuesta->index, respuesta->begin, respuesta->length);
		    
		    requests.hold();
		    int size = requests.size();
		    DownloadSlot *ds = NULL;
		    for(int i=0;i<size;i++){
			 ds = requests.popFront();
			 if(requests.isValid() && ds != NULL &&		\
			    respuesta->index == ds->getPieceIndex() &&	\
			    respuesta->begin == ds->getOffset()     &&	\
			    respuesta->length == ds->getLength()){
			      break;
			 }
			 else{
			      requests.push(ds);
			      ds = NULL;
			 }
		    }
		    requests.release();
		    downloaded += respuesta->length;
		    if(ds != NULL)
			 torrent->peerTransferFinished(this,ds);
		    else stop();
		    break;
	       }
	       case CANCEL:
	       
		    break;
	       case PORT:
	       
		    break;
	       case KEEP_ALIVE:
	       
		    break;

	       default:
		    std::cout << "Lucia, deja de mandar mensajes sin sentido!!! @ " << socket << "\n";
		    stop();
		    break;
	       }
	  }
     }

     std::cout << "FUERA PEER\n";

     conectado = false;
     colaDatos=NULL;
     datos->release();
     receptor->finish();
     emisor->finish();
     socket->cerrar();
     delete receptor;
     delete emisor;
     delete socket;

     /* Le aviso al torrent que finalizo este peer */
     torrent->eliminarPeer(this);
}


void Peer::finish(){
     Thread::finish();
}

Peer::~Peer(){
     if(colaDatos)
	  colaDatos->invalidate();
}
