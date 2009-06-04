#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include "torrent.h"
#include <iostream>
#include <list>

/* Modelo del cliente torrent */
class Client{
private:
     std::string nickUsuario;
     std::list<Torrent*> torrents;

public:
     /* Crea un nuevo cliente */
     Client();
     
     /* Agrega un torrent */
     int addTorrent(const char* file);

     /* comienza a descargar un torrent */
     int start();
};

#endif /* CLIENT_H_INCLUDED */
