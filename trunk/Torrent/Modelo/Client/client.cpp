#include "client.h"

/****************************************************************************/
bool Client::addTorrent(const char* path){
	
	Torrent *torrent = new Torrent(path);
	
    if(torrent->isValid()){
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

	/* por ahora, nada mas para probar comienza solo el primer torrent */
    Torrent* torrent = torrents.front(); 
    //Torrent* torrent= buscarTorrent(filename);

//    return torrent->start();
//    if(torrent) {
//		torrent->start();
//		return true;
//	} else
//		return false;	
}

/*--------------------------------------------------------------------------*/
bool Client::stop(const char* filename) {
	
	Torrent* torrent= buscarTorrent(filename);
	if(torrent) {
		torrent->stop();
		return true;
	} else
		return false;	
}
     
/*--------------------------------------------------------------------------*/
bool Client::pause(const char* filename) {
	
	Torrent* torrent= buscarTorrent(filename);
	if(torrent) {
		torrent->pause();
		return true;
	} else
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
	} else
		return false;		
	
}

/*--------------------------------------------------------------------------*/
Torrent* Client::buscarTorrent(const char* filename) {
	
	std::list<Torrent*>::iterator it;
	bool encontrado= false;
	
    for(it= torrents.begin(); it!= torrents.end() && !encontrado; it++) {     	
     	if(strcmp(filename, (*it)->getName().c_str()) == 0)
     		encontrado= true;	
     }
     
    if(!encontrado)
    	return NULL;
  	else   
    	return *(--it);
}

/*--------------------------------------------------------------------------*/
void Client::setBitFieldTorrent(const char* filename, char* data) {
	
	Torrent* torrent= buscarTorrent(filename);
	torrent->getBitField()->setData(data);
}

/*--------------------------------------------------------------------------*/
std::list<Torrent*>* Client::getListaTorrents(){
	return &torrents;
}

/*--------------------------------------------------------------------------*/
Client::~Client(){

	std::list<Torrent*>::iterator it;
    for(it=torrents.begin();it!=torrents.end();it++){
		(*it)->stop();
		delete (*it);
    }
}

/****************************************************************************/
