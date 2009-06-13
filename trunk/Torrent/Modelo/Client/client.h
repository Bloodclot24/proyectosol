#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include "torrent.h"
#include <iostream>
#include <list>


#define CLIENT_ID "-SN0100-abcdefghijkl"

/* Modelo del cliente torrent */
class Controlador;
class Client{
private:
     std::string nickUsuario;
     std::list<Torrent*> torrents;
     Controlador* controlador;

public:
     /* Agrega un torrent */
     int addTorrent(const char* file);

     /* comienza a descargar un torrent */
     int start();

     /* Destruye el cliente y libera todos los torrents */
     ~Client();
};

#endif /* CLIENT_H_INCLUDED */
