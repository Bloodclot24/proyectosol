#ifndef TORRENT_H_INCLUDED
#define TORRENT_H_INCLUDED

class Peer;

#include "socket.h"
#include "threadEmisor.h"
#include "threadReceptor.h"

#include "torrentFile.h"
#include "../ParserBencode/parserBencode.h"
#include "peer.h"
#include "bitField.h"

#include <iostream>
#include <limits.h>

enum EstadoTorrent {STOPPED, PAUSED, DOWNLOADING, SEEDING};

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

     std::string idHash;

     EstadoTorrent estado;

     Socket* socket;
     ThreadReceptor *receptor;
     ThreadEmisor *emisor;

     bool valido;
     
     std::list<Peer*> listaPeers;

     BitField *bitField; // BitField que representa las piezas que tenemos
     
     uint32_t rarestFirst();

public:
     Torrent(const char* fileName);

     /* Comienza el torrent */
     int start();

     /* Devuelve el tamao total de todos los archivos contenidos en
      * el torrent */
     int getTotalSize();

     /* Indica si el objeto es valido o no */
     bool isValid();

     BitField* getBitField();

     ~Torrent();
};

#endif /* TORRENT_H_INCLUDED */
