#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include "torrent.h"
#include <iostream>
#include <list>
#include <string.h>
#include "../ParserBencode/parserBencode.h"
#include "threadAceptor.h"
#include "socket.h"

class Client;
#include "../../Controlador/controlador.h"

#define CLIENT_ID      "-TR010-abcdefghijklm"
#define PORT_IN        12345
#define PEERS_NUM_WANT 50
#define MAX_REQUESTS   10

/* Modelo del cliente torrent */
class Client {
private:
     std::string nickUsuario;
     std::list<Torrent*> torrents;
     Controlador* controlador;
     
     Socket* socket;
     ThreadAceptor* threadAceptor;
     
     uint32_t puertoPorDefecto;
     uint32_t cantidadDePeers;
     
     Torrent* buscarTorrent(const char* filename);

public:

	Client(Controlador* controlador);
	
     /* agrega un torrent */
    bool addTorrent(const char* path, BitField* bitfield= NULL);

     /* comienza a descargar un torrent */
    bool start(const char* filename);

	/* detiene la descarga de un torrent */
    bool stop(const char* filename);
     
	/* suspende la descarga de un torrent */
    bool pause(const char* filename);

	/* borra la descarga de un torrent */
    bool remove(const char* filename);

	//TODO: murio esto :p
//	/* modifica el bitfield del torrent */
//	void setBitFieldTorrent(const char* filename, char* data);

	/* obtiene la lista de torrents */
	const std::list<Torrent*>* getListaTorrents();
	
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
class Client;
