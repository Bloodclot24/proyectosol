#include "client.h"

/****************************************************************************/
Client::Client(Controlador* controlador) {
	
	this->controlador= controlador;
	this->puertoPorDefecto = PORT_IN;
	this->cantidadDePeers = PEERS_NUM_WANT;
	this->socket = new Socket("localhost",PORT_IN);
	this->threadAceptor = new ThreadAceptor(this->socket);
}

/*--------------------------------------------------------------------------*/
bool Client::addTorrent(const char* path, BitField* bitfield){
	
     Torrent *torrent = new Torrent(path, bitfield);
	
     if(torrent->isValid()){
	  torrent->setControlador(controlador);
	  torrents.push_back(torrent);
	  return true;
     }
     else {
	  delete torrent;
	  return false;
     }
}

/*--------------------------------------------------------------------------*/
bool Client::start(const char* filename){
	
	 // comienzo a recibir conexiones de peers entrantes
//	 this->threadAceptor->comenzar();
	 
//     Torrent* torrent= buscarTorrent(filename);
//     EstadoTorrent estado= torrent->getEstadoTorrent(); 
//     if(torrent && (estado ==  PAUSED || estado ==  STOPPED)) {
//	  /* por ahora, nada mas para probar comienza solo el primer torrent */
	  Torrent* torrent = torrents.front(); 
//	  //Torrent* torrent= buscarTorrent(filename);
//	return 0;  
	
		//COMENTE ASI NO CORRE
		

	 return torrent->start();
//    if(torrent) {
//		torrent->start();
//		return true;
//	} else
//		return false;	
//	}
}

/*--------------------------------------------------------------------------*/
bool Client::stop(const char* filename) {
	
	Torrent* torrent= buscarTorrent(filename);
	if(torrent) {
		if(torrent->stop())
			return true;
	}
	return false;	
}
     
/*--------------------------------------------------------------------------*/
bool Client::pause(const char* filename) {
	
	Torrent* torrent= buscarTorrent(filename);
	if(torrent) {
		if(torrent->pause())
			return true;
	} 
	return false;	
}

/*--------------------------------------------------------------------------*/
bool Client::remove(const char* filename) {
	
	Torrent* torrent= buscarTorrent(filename);
	if(torrent) {
		torrent->stop();
		torrents.remove(torrent);
		delete torrent;
		return true;
	}
	return false;		
}

/*--------------------------------------------------------------------------*/
Torrent* Client::buscarTorrent(const char* filename) {
	
     std::list<Torrent*>::iterator it;
     bool encontrado= false;
     
     for(it= torrents.begin(); it!= torrents.end() && !encontrado; it++) {     	
	  if(strcmp(filename, (*it)->getName().c_str()) == 0){
	       encontrado = true;
		    break;
	  }
     }
     
     if(!encontrado)
	  return NULL;
     else   
	  return *(it);
}

/*--------------------------------------------------------------------------*/
//void Client::setBitFieldTorrent(const char* filename, char* data) {
//	
//	Torrent* torrent= buscarTorrent(filename);
//	torrent->getBitField()->setData(data);
//}

/*--------------------------------------------------------------------------*/
const std::list<Torrent*>* Client::getListaTorrents(){
	return &torrents;
}

/*--------------------------------------------------------------------------*/
uint32_t Client::getPortDefault(){
	return puertoPorDefecto;
}

/*--------------------------------------------------------------------------*/
void Client::setPortDefault(uint32_t puerto){
	this->puertoPorDefecto =  puerto;
}

/*--------------------------------------------------------------------------*/
uint32_t Client::getNumPeersForTorrent(){
	return cantidadDePeers;
}

/*--------------------------------------------------------------------------*/
void Client::setNumPeersForTorrent(uint32_t numeroDePeers){
	this->cantidadDePeers =  numeroDePeers;
}

/*--------------------------------------------------------------------------*/
Client::~Client(){

	std::list<Torrent*>::iterator it;
    for(it=torrents.begin();it!=torrents.end();it++){
		(*it)->stop();
		delete (*it);
    }
    // dejo de recibir conexiones de peers entrantes
    this->threadAceptor->finish();
    delete this->threadAceptor;
}

/****************************************************************************/
