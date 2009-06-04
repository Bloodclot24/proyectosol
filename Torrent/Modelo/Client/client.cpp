#include "client.h"
#include "torrent.h"
#include "../ParserBencode/parserBencode.h"

#include <iostream>
#include <list>


Client::Client(){

}

/****************************************************************************/
int Client::addTorrent(const char* file){
     ParserBencode parser;
     /* Decodifico todo el .torrent y obtengo una lista con toda la
      * informacion */
     std::list<BeNode*> *lista = parser.beDecode(file);

     if(lista!= NULL){
	  Torrent *torrent = new Torrent(lista);
	  torrents.push_back(torrent);
	  /* TODO: liberar cada elemento antes de eliminar */
	  delete lista;
	  return 1;
     }

     else return 0;
}

/****************************************************************************/
int Client::start(){
     /* por ahora, nada mas para probar comienza solo el promer torrent */
     Torrent* torrent = torrents.front();

     torrent->start();
     

}
