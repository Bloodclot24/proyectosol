#ifndef TORRENT_H_INCLUDED
#define TORRENT_H_INCLUDED

class Peer;

#include "socket.h"
#include "threads.h"
#include "threadEmisor.h"
#include "threadReceptor.h"

#include "torrentFile.h"
#include "../ParserBencode/parserBencode.h"
#include "peer.h"
#include "bitField.h"
#include "threads.h"

#include <iostream>
#include <limits.h>
#include <math.h>

/* Estados posibles del torrent */
enum EstadoTorrent {STOPPED, PAUSED, DOWNLOADING, SEEDING, ERROR};

/* El modelo de cada Torrent que maneja el cliente */
class Torrent:public Thread{
private:
     /* El nombre del archivo .torrent */
     std::string nombreTorrent;

     /* La lista de archivos del torrent */
     std::list<TorrentFile*>* archivos; 

     /* URL del announce del tracker. TODO: considerar lista de URL's */
     std::string announceUrl;

     /* Fecha de creacion */
     int creationDate;
     
     /* Algun comentario */
     std::string comment;

     /* Creador */
     std::string createdBy;

     /* Codificacion */
     int encoding;

     std::string idHash;

     /* estado actual del torrent */
     EstadoTorrent estado;

     Socket* socket;
     ThreadReceptor *receptor;
     ThreadEmisor *emisor;

     /* indica si es valido el torrent */
     bool valido;
     
     /* Peers asociados a este torrent */
     std::list<Peer*> listaPeers;
     
     Mutex mutexPeers;

     BitField *bitField; // BitField que representa las piezas que tenemos

     /* representa las piezas que estamos bajando. Como cada pieza se
      * baja por partes, esto nos indica que partes de cada pieza
      * tenemos y que partes nos faltan */
     std::map<uint32_t, BitField*> piezasEnProceso;

     /* numero de peers activos */
     int peersActivos;

     Mutex requestMutex;
     CVariable requestCondition;

     /* Cola con los peers que nos enviaron un Unchoke */
     Deque<Peer*> peersEnEspera;

     Mutex downloadMutex;

     uint32_t sizeInPieces;
     uint32_t pieceSize;

private:

     /* Devuelve el numero de pieza que esmas dificil de conseguir (de
      * las que no tenemos) */
     uint32_t rarestFirst();

public:
     Torrent(const char* fileName);
     Torrent(const char* fileName, char* bitfield);

     /* Comienza el torrent */
     int start();

     /* Devuelve el tamao total de todos los archivos contenidos en
      * el torrent */
     uint64_t getTotalSize();

     /* Devuelve el nombre del .torrent original*/
     const std::string& getName(){ return nombreTorrent; }

     /* Indica si el objeto es valido o no */
     bool isValid();

     /* Devuelve el bitField asociado al torrent */
     BitField* getBitField();

     /* Aade un peer a la lista de peers cuidando que no este repetido */
     void agregarPeer(Peer* peer);
     
     /* Elimina un peer de la lista de peers */
     void eliminarPeer(Peer* peer);

     /* Rutina principal del torrent. Aqui se maneja la logica */
     virtual void run();

     /*Dado un indice, obtiene el FileTorrent al que corresponde*/
     TorrentFile* obtenerArchivo(uint32_t index);

     /* Dado un indice devuelve el offset dentro del archivo donde cae
      * esa pieza */
     uint64_t obtenerByteOffset(uint32_t index);

     /* escribe un bloque de datos en el/los archivo/archivos que corresponda/n */
     int writeData(const char* data, uint32_t index, uint32_t offset, uint32_t size);

     /* lee un bloque de datos de el/los archivo/archivos que corresponda/n */
     int readData(char* data, uint32_t index, uint32_t offset, uint32_t size);

     /* Metodo que llaman los peers cuando se conectan exitosamente */
     void peerConected(Peer *peer);

     /* Metodo que llaman los peers envian 'choke' */
     void peerChoked(Peer* peer);

     /* Metodo que llaman los peers envian 'unchoke' */
     void peerUnchoked(Peer* peer);

     /* Avisa a todos los peers conectados que tenemos una pieza */
     void anunciarPieza(uint32_t index);

     /* Realiza un announce al tracker */
     int announce();

     /* Realiza un scrape */
     int scrape();

     /* Indica si una pieza es o no valida */
     int validarPieza(uint32_t index);

     int stop(){ return 0; }
     int pause(){ return 0; }
     int remove(){ return 0; }
     
     /* Devuelve el estado del torrent */
     EstadoTorrent getEstadoTorrent() { return estado; };

     ~Torrent();
};

#endif /* TORRENT_H_INCLUDED */
