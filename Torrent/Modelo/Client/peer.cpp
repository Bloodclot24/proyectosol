#include "peer.h"
#include "threadReceptor.h"
#include "threadEmisor.h"
#include "client.h"
#include "mensaje.h"
#include "signal.h"

/****************************************************************************/
Peer::Peer(const std::string& host, int puerto , Torrent* torrent){
     port = puerto;
     name = host;
     am_choking = 1;
     am_interested = 0;
     peer_choking = 1;
     peer_interested = 0;
     this->torrent = torrent;
     this->bitField = new BitField(ceil(torrent->getBitField().getLength()));
     conectado = true;
     this->entrante = false;
     downloaded = 0;
     uploaded = 0;
     corrupted = 0;
     colaDatos = NULL;
     receptor = NULL;
     emisor = NULL;
     socket = NULL;
     estado = PEER_WAITING;
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
     conectado = true;
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
     return peer_choking;
}

/****************************************************************************/
void Peer::sendRequest(uint32_t index, uint32_t offset,		\
		       uint32_t size=REQUEST_SIZE_DEFAULT){
     
     Mensaje *mensaje = new Mensaje();
     ProtocoloBitTorrent proto;
     std::string msg = proto.request(index, offset, size);
     mensaje->copiarDatos(msg.c_str(), msg.length());
     emisor->enviarMensaje(mensaje);
}
/****************************************************************************/
void Peer::sendRequest(DownloadSlot* ds){
     Mensaje *mensaje = new Mensaje();
     ProtocoloBitTorrent proto;
     std::string msg = proto.request(ds->getPieceIndex(), ds->getOffset(), ds->getLength());
     mensaje->copiarDatos(msg.c_str(), msg.length());
     requests.push(ds);
     emisor->enviarMensaje(mensaje);
}

/****************************************************************************/
bool Peer::havePiece(uint32_t index){
     return bitField->getField(index);
}

/****************************************************************************/
void Peer::run(){
     estado = PEER_CONNECTING;
     ProtocoloBitTorrent proto;
     Deque<char> *datos = NULL;
     Mensaje *mensaje = NULL;
     conectado = true;
     if(!this->entrante){
	  this->socket = new Socket(name,port);
	  this->emisor = new ThreadEmisor(this->socket);
	  this->receptor = new ThreadReceptor(this->socket,false);
	  
//	  socket->setTimeout(30,0);
	  socket->conectar();
	  if(!socket->esValido()){
	       stop();
	       std::cout <<  "ERROR DEL SOCKET: " << socket->obtenerError()  << "\n";
	       conectado = false;
	       return;
	  }
	  
	  // "CONECTADO EXITOSAMENTE 
	  receptor->comenzar();
	  emisor->comenzar();
	  
	  /* Envio un handshake */
	  mensaje = new Mensaje();
	  std::string aux = proto.handshake("BitTorrent protocol", hash, CLIENT_ID);
	  mensaje->copiarDatos(aux.c_str(), aux.length());
	  emisor->enviarMensaje(mensaje);
	  
	  /* Envio un bitfield con las piezas que ya tenemos */
	  mensaje = new Mensaje();
	  aux = proto.bitfield(torrent->getBitField().getBytesLength());
	  mensaje->copiarDatos(aux.c_str(), aux.length());
	  emisor->enviarMensaje(mensaje);
	  mensaje = new Mensaje();
	  mensaje->copiarDatos(torrent->getBitField().getData(), torrent->getBitField().getBytesLength());
	  emisor->enviarMensaje(mensaje);
	  
	  /* Recibo el handshake del peer */
	  colaDatos = datos = receptor->getColaDeDatos();
	  for(int i = 0; i <49+19 && estado==PEER_CONNECTING;i++)
	       datos->popFront();
	  conectado = true;
	  
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
	  aux = proto.bitfield(torrent->getBitField().getBytesLength());
	  mensaje->copiarDatos(aux.c_str(), aux.length());
	  emisor->enviarMensaje(mensaje);
	  mensaje = new Mensaje();
	  mensaje->copiarDatos(torrent->getBitField().getData(), torrent->getBitField().getBytesLength());
	  emisor->enviarMensaje(mensaje);
     }


     if(conectado && torrent){
	  /* Aviso al torrent que me pude conectar */
	  torrent->peerConected(this);
	  estado = PEER_RUNNING;
     }
     else
	  stop();
     
     while(estado == PEER_RUNNING && receptor !=NULL && emisor!=NULL &&receptor->isRunning() ){
	  Message *respuesta = proto.decode(*datos);
// 	  if(!datos->isValid()){
// 	       stop();
// 	  }
// 	  else{
	  
	  if(estado == PEER_RUNNING){
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
		    std::cout << "bitfield\n";
		    if(respuesta->length == torrent->getBitField().getBytesLength()) {
			 uint32_t i;
			 for(i=0; i<(respuesta->length) ;i++)
			      bitField->setBlock(datos->popFront(),i);
			 
// 			 if(i < respuesta->length){
// 			      estado = PEER_STOPPING;
// 			      std::cout << "FRENANDO BITFIELDDDDDDDDDDD\n";
// 			      stop();
// 			 }
			
		    }
		    else{
			 //Cerrar conexion y salir
			 // "Bitfield de longitud erronea recibido"
			 estado = PEER_STOPPING;
			 std::cout << "FRENANDO BITFIELDDDDDDDDDDD222\n";
			 stop();
		    }
		    break;
	       case REQUEST:
		    std::cout << "REQUESTTTTTTTTTTTTTTTTTTTTTTT\n";
		    if(torrent->getBitField().getField(respuesta->index) ==1){
			 std::cout << "request\n";
			 //OK, me piden una pieza que tengo
			 //Genero el mensaje del protocolo
			 std::string aux = proto.piece(respuesta->index, respuesta->begin, respuesta->length);
			 mensaje = new Mensaje();
			 mensaje->copiarDatos(aux.c_str(), aux.length());
			 emisor->enviarMensaje(mensaje);
			 		    
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
		    std::cout << "piece\n";
		    std::string bloque;
		    while(bloque.length() < respuesta->length && estado == PEER_RUNNING){
			 bloque += datos->popFront();
		    }
		    if(estado == PEER_RUNNING)
			 torrent->writeData(bloque.c_str(), respuesta->index, respuesta->begin, bloque.length());
		    
		    int size = requests.size();
		    DownloadSlot *ds = NULL;
		    for(int i=0;i<size;i++){
			 ds = requests.popFront();
			 if(ds != NULL &&		\
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

		    if(ds && torrent){
			 downloaded += respuesta->length;
			 if(bloque.length() == respuesta->length)
			      torrent->peerTransferFinished(this,ds);
			 else
			      torrent->peerTransferCanceled(this,ds);
		    }
		    else{
			 estado = PEER_STOPPING;
			 std::cout << "FRENANDO PIECEEEE\n";
			 stop();
		    }
		    break;
	       }
	       case CANCEL:
	       
		    break;
	       case PORT:
	       
		    break;
	       case KEEP_ALIVE:
	       
		    break;

	       default:
		    std::cout << "FRENANDO por inanicion \n";
		    estado = PEER_STOPPING;
		    stop();
		    break;
	       }
	  }
     }
	  
     colaDatos=NULL;
	  
     conectado = false;
}


void Peer::finish(){
     estado = PEER_STOPPING;
     stop();
     signal();
     if(receptor){
	  receptor->finish();
	  std::cout << "FINALIZANDO EL RECEPTORRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR" << this <<"\n";
     }
     if(emisor){
	  emisor->finish();
     }

     signal();

     if(socket)
	  socket->cerrar();
     Thread::finish();
     
     if(torrent != NULL){
	  DownloadSlot *ds = NULL;
	  while(requests.size()>0){
	       ds = requests.popFront();
	       torrent->abortRequest(ds);
	  }
	  torrent = NULL;
     }
     std::cout << "JOIIIIIIIIIIIIIIIIIIIIIIIINNNNNNNNNN" << this <<"\n";
     join();
     estado = PEER_FINISHED;
}


uint32_t Peer::getVelBajada(){
	
	return socket->getVelBajada();
}
	
uint32_t Peer::getVelSubida(){

	return socket->getVelSubida();	
}

Peer::~Peer(){
     std::cout << "borrando peer\n";
     finish();
     while(estado != PEER_FINISHED);
     if(receptor)
	  delete receptor;
     if(emisor)
	  delete emisor;
     if(socket)
	  delete socket;
}
