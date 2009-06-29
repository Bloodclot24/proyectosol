#ifndef TORRENTFILE_H_INCLUDED
#define TORRENTFILE_H_INCLUDED

#include "../ParserBencode/parserBencode.h"
#include "../FileManager/fileManager.h"

#include <iostream>

/** 
 * Clase que representa uno de los archivos contenidos en el torrent.
 */
class TorrentFile{
private:
     bool isPrivate; 
     std::string name;		/**< Nombre del archivo en disco */


     std::string pieces;	/**< SHA1 de las piezas totales y
				 * tamao de c/u. Si el torrent es
				 * multiarchivo, este atributo esta
				 * presente solamente en el primer
				 * torrentFile de la lista. */

     int pieceLength;		/**< Tamaño de las piezas en bytes */

     uint64_t length;		/**< Longitud del archivo en bytes */

     std::string md5;		/**< MD5 del archivo, actualmente no
				 * se usa */

     std::string path;		/**< Path del archivo en disco. */
     
     FileManager *manager;	/**< FileManager para manejar el archivo */
     
private:
     /** 
      * Crea un TorrentFile. Es privado para evitar la creacion
      * externa de torrentFile. Para crear TorrentFile debe utilizarse
      * la funcion Parse().
      */
     TorrentFile(){};

public:
     /** 
      * Dado un diccionario 'informacion' codificado en BEncode, crea
      * un lista de TorrentFile.
      * 
      * @param node El diccionario a decodificar.
      * 
      * @return La lista de TorrentFile creada.
      */
     static std::list<TorrentFile*>* Parse(BeNode* node);
     
     /** 
      * Devuelve los hashes de todas las piezas.
      *
      * @return String conteniendo los SHA1 de cada pieza.
      *
      * @see Sha1
      */
     const std::string& getPieces(){ return pieces; }

     /** 
      * Devuelve el tamanio del archivo.
      * 
      * @return El tamaño del archivo.
      */
     uint64_t getSize(){ return length; }
     
     /** 
      * Devuelve el tamanio de las piezas.
      * 
      * @return el tamanio de las piezas.
      */
     int getPieceLength(){ return pieceLength;}

     /** 
      * Devuelve el FileManager asociado al torrentFile.
      * 
      * @return El FileManager para manipular el archivo.
      */
     FileManager* getManager(){ return manager; }

     /** 
      * Devuelve una referencia a la 'tira' de hashes.
      * 
      * @return String conteniendo los SHA1 de cada pieza.
      *
      * @see Sha1
      */
     const std::string& getHashes(){ return pieces; }

     /** 
      * Destruye el TorrentFile, cierra el archivo asociado y libera
      * la memoria.
      */
     ~TorrentFile();

};

#endif /* TORRENTFILE_H_INCLUDED */
