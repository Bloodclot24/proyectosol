#ifndef TORRENT_H_INCLUDED
#define TORRENT_H_INCLUDED

#include "socket.h"
#include "threadEmisor.h"
#include "threadReceptor.h"

#include "torrentFile.h"
#include "../ParserBencode/parserBencode.h"

#include <iostream>

typedef enum EstadoTorrent {STOPPED, PAUSED, DOWNLOADING, SEEDING};

/* El modelo de cada Torrent que maneja el cliente */
class Torrent{
private:
     /* El nombre del archivo .torrent */
     std::string nombreTorrent;
     /* La lista de archivos del torrent */
     std::list<TorrentFile*>* archivos; 

     /* URL del announce del tracker. TODO: considerar lista de URL's */
     std::string announce;

     /* Fecha de creacion */
     int creationDate;
     
     /* Algun comentario */
     std::string comment;

     /* Creador */
     std::string createdBy;

     /* Codificacion */
     int encoding;

     EstadoTorrent estado;

     Socket* socket;
     ThreadReceptor *receptor;
     ThreadEmisor *emisor;

public:
     Torrent(std::list<BeNode*> *info);

     /* Comienza el torrent */
     int start();

     /* Devuelve el tama�o total de todos los archivos contenidos en
      * el torrent */
     int getTotalSize();
};

#endif /* TORRENT_H_INCLUDED */
