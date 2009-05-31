#ifndef TORRENTFILE_H_INCLUDED
#define TORRENTFILE_H_INCLUDED

#include <iostream>

/* Clase que representa uno de los archivos contenidos en el torrent */
class TorrentFile{
private:
     bool isPrivate;
     std::string name;

     /* SHA1 de las piezas totales y tamaño de c/u */
     std::string pieces;
     int pieceLength;

     /* Longitud del archivo en bytes */
     int length;

     std::string md5;

public:
     TorrentFile(BeNode* node);

};

#endif /* TORRENTFILE_H_INCLUDED */
