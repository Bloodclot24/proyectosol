#include "client.h"
#include "torrent.h"
#include "../ParserBencode/parserBencode.h"

#include <iostream>
#include <list>


/****************************************************************************/
int Client::addTorrent(const char* file){
     Torrent *torrent = new Torrent(file);
     if(torrent->isValid()){
	  torrents.push_back(torrent);
	  return 1;
     }
     else {
	  delete torrent;
	  return 0;
     }
}

/****************************************************************************/
int Client::start(){
     /* por ahora, nada mas para probar comienza solo el primer torrent */
     Torrent* torrent = torrents.front();

     return torrent->start();
}
