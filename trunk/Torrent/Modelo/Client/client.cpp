#include "client.h"

/****************************************************************************/
Client::Client(Controlador* controlador): trackersIds() {

	this->controlador= controlador;
	this->puertoPorDefecto = PORT_IN;
	this->cantidadDePeers = PEERS_NUM_WANT;
	this->socket = new Socket("localhost",PORT_IN);
	this->threadAceptor = new ThreadAceptor(this->socket);
}

/*--------------------------------------------------------------------------*/
bool Client::addTorrent(const char* path, BitField* bitfield){

     Torrent *torrent = new Torrent(path, this, bitfield);

     if(torrent->isValid()){
	  torrent->setControlador(controlador);
	  torrents.push_back(torrent);
	  return true;
     }
     else {
	  //TODO: pincha con los torrents vacios
	  delete torrent;
	  return false;
     }
}

/*--------------------------------------------------------------------------*/
bool Client::start(const char* filename){

     // comienzo a recibir conexiones de peers entrantes
     if(!this->threadAceptor->isRunning())this->threadAceptor->comenzar();

     Torrent* torrent= buscarTorrent(filename);


     if(torrent) {
	  return torrent->start();
     } else
	  return false;
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
//		delete torrent;
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
bool Client::existeTorrent(const char* filename){

	bool encontrado= false;
	std::list<Torrent*>::iterator it;

	for(it= torrents.begin(); it!= torrents.end() && !encontrado; it++) {
		if(strcmp(filename, (*it)->getName().c_str()) == 0){
	    	encontrado = true;
		    break;
	  	}
   	}

	if(encontrado)
		return true;
	return false;
}

/*--------------------------------------------------------------------------*/
bool Client::existeTracker(const std::string tracker) {

	 std::string trackerMap= trackersIds[tracker];
     if((trackerMap.compare("")) != 0)
     	return true;
     return false;
}

/*--------------------------------------------------------------------------*/
const std::string Client::trackerId(const std::string tracker) {

	return trackersIds[tracker];
}

/*--------------------------------------------------------------------------*/
void Client::addTracker(const std::string tracker, const std::string id) {

	trackersIds.insert(std::pair<std::string,std::string>(tracker, id));
}

/*--------------------------------------------------------------------------*/
void Client::modificarIdTracker(const std::string tracker, const std::string id) {

	trackersIds[tracker]= id;
}

/*--------------------------------------------------------------------------*/
const std::string Client::getClientId(){
	return CLIENT_ID;
}

/*--------------------------------------------------------------------------*/
const std::string Client::getPortIn(){
	return intAstring(PORT_IN);
}

/*--------------------------------------------------------------------------*/
const std::string Client::getPeersNumWant(){
	return intAstring(PEERS_NUM_WANT);
}

/*--------------------------------------------------------------------------*/
const std::string Client::getMaxRequest(){
	return intAstring(MAX_REQUESTS);
}

/*--------------------------------------------------------------------------*/
Torrent* Client::buscarPorHash(std::string hash) {

     std::list<Torrent*>::iterator it;
     bool encontrado= false;

     for(it= torrents.begin(); it!= torrents.end() && !encontrado; it++) {
	  if(hash.compare((*it)->getIdHash()) == 0){
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
Client::~Client(){

     std::list<Torrent*>::iterator it;
     for(it=torrents.begin();it!=torrents.end();it++){
	  (*it)->stop();
	  delete (*it);
     }
     // dejo de recibir conexiones de peers entrantes
     this->threadAceptor->finish();
     this->threadAceptor->signal();
     delete this->threadAceptor;
}

/****************************************************************************/
