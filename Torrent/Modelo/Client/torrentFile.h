#ifndef TORRENTFILE_H_INCLUDED
#define TORRENTFILE_H_INCLUDED

#include "../ParserBencode/parserBencode.h"

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

     std::string path;

     TorrentFile(){};

public:
     static std::list<TorrentFile*>* Parse(BeNode* node);
     
     /* Devuelve los hashes de todas las piezas */
     const std::string& getPieces(){ return pieces; }

     /* Devuelve el tamaño del archivo */
     int getSize(){ return length; }

     ~TorrentFile();

};

#endif /* TORRENTFILE_H_INCLUDED */
