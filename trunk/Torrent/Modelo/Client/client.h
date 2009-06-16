#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include "torrent.h"
#include <iostream>
#include <list>
#include <string.h>
#include "../ParserBencode/parserBencode.h"

#define CLIENT_ID      "-SN010-abcdefghijkl"
#define PORT_IN        12345
#define PEERS_NUM_WANT 50

/* Modelo del cliente torrent */
class Controlador;
class Client{
private:
     std::string nickUsuario;
     std::list<Torrent*> torrents;
     Controlador* controlador;
     
     uint32_t puertoPorDefecto;
     uint32_t cantidadDePeers;
     
     Torrent* buscarTorrent(const char* filename);

public:

	Client();
	
     /* agrega un torrent */
    bool addTorrent(const char* path);

     /* comienza a descargar un torrent */
    bool start(const char* filename);

	/* detiene la descarga de un torrent */
    bool stop(const char* filename);
     
	/* suspende la descarga de un torrent */
    bool pause(const char* filename);

	/* borra la descarga de un torrent */
    bool remove(const char* filename);

	/* modifica el bitfield del torrent */
	void setBitFieldTorrent(const char* filename, char* data);

	/* obtiene la lista de torrents */
	std::list<Torrent*>* getListaTorrents();
	
	/* obtiene el puerto de conexion por defecto */
	uint32_t  getPortDefault();
	
	/* modifica el puerto de conexion por defecto */
	void setPortDefault(uint32_t puerto);
	
	/* obtiene el numero de peers para un torrent */
	uint32_t getNumPeersForTorrent();
	
	/* modifica el numero de peers para un torrent */
	void setNumPeersForTorrent(uint32_t numeroDePeers);
	
    /* destruye el cliente y libera todos los torrents */
    ~Client();
};

#endif /* CLIENT_H_INCLUDED */
