#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include "torrent.h"
#include <iostream>
#include <list>
#include <string.h>
#include "../ParserBencode/parserBencode.h"

#define CLIENT_ID "-SN0100-abcdefghijkl"

/* Modelo del cliente torrent */
class Controlador;
class Client{
private:
     std::string nickUsuario;
     std::list<Torrent*> torrents;
     Controlador* controlador;
     
     Torrent* buscarTorrent(const char* filename);

public:
     /* Agrega un torrent */
     bool addTorrent(const char* file);

     /* comienza a descargar un torrent */
     bool start(const char* filename);

	/* detiene la descarga un torrent */
     bool stop(const char* filename);
     
	/* suspende la descarga un torrent */
     bool pause(const char* filename);

	/* borrar la descarga un torrent */
     bool remove(const char* filename);

     /* Destruye el cliente y libera todos los torrents */
     ~Client();
};

#endif /* CLIENT_H_INCLUDED */
