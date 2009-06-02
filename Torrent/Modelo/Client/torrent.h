#ifndef TORRENT_H_INCLUDED
#define TORRENT_H_INCLUDED

#include "torrentFile.h"
#include "../ParserBencode/parserBencode.h"

#include <iostream>

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
public:
     Torrent(std::list<BeNode*> *info);

};

#endif /* TORRENT_H_INCLUDED */
