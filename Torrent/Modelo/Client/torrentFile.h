#ifndef TORRENTFILE_H_INCLUDED
#define TORRENTFILE_H_INCLUDED

#include "../ParserBencode/parserBencode.h"
#include "../FileManager/fileManager.h"

#include <iostream>

/* Clase que representa uno de los archivos contenidos en el torrent */
class TorrentFile{
private:
     bool isPrivate;
     std::string name;

     /* SHA1 de las piezas totales y tamao de c/u. Si el torrent es
      * multiarchivo, este atributo esta presente solamente en el
      * primer torrentFile de la lista. */
     std::string pieces;

     int pieceLength;

     /* Longitud del archivo en bytes */
     uint64_t length;

     std::string md5;

     std::string path;
     
     FileManager *manager;
     
private:
     TorrentFile(){};

public:
     static std::list<TorrentFile*>* Parse(BeNode* node);
     
     /* Devuelve los hashes de todas las piezas */
     const std::string& getPieces(){ return pieces; }

     /* Devuelve el tamanio del archivo */
     uint64_t getSize(){ return length; }
     
     /* Devuelve el tamanio de las piezas */
     int getPieceLength(){ return pieceLength;}

     /* Devuelve el FileManager asociado al torrentFile */
     FileManager* getManager(){ return manager; }

     /* Devuelve una referencia a la 'tira' de hashes */
     const std::string& getHashes(){ return pieces; }

     ~TorrentFile();

};

#endif /* TORRENTFILE_H_INCLUDED */
