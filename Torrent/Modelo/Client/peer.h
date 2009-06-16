#ifndef PEER_H_INCLUDED
#define PEER_H_INCLUDED

class Torrent;

#define REQUEST_SIZE_DEFAULT 16*1024

#include "socket.h"
#include "../ProtocoloBitTorrent/protocoloBitTorrent.h"
#include "threads.h"
#include "torrent.h"
#include "bitField.h"

class Peer:public Thread{
private:
     Socket socket;
     ThreadEmisor emisor;
     ThreadReceptor receptor;
     
     Torrent* torrent;

     int am_choking, am_interested, peer_choking, peer_interested;

     std::string hash;

     bool corriendo;
     
     BitField *bitField;

     std::string name;
     uint32_t port;

protected:
     void run();
     
public:
     bool conectado;
     /* Dado un peer en forma compacta (ip.ip.ip.ip:puerto), crea un
      * nuevo peer */
     Peer(const std::string& host, int puerto, Torrent* torrent);

     /* Devuelve un bitField con las piezas del peer */
     const BitField* getBitField();

     /* Devuelve el nombre del peer */
     const std::string& getName();

     /* Devuelve el thread emisor del peer */
     ThreadEmisor* getEmisor();

     /* Cambia el estado de am_interested */
     void setInterested(bool);
     
     /* Obtiene el estado de am_interested */
     bool getInterested();

     /* Cambia el estado de am_choking */
     void setChoke(bool);

     /* Obtiene el estado de am_choking */
     bool getChoke();

     /* Envia un request de una parte */
     void sendRequest(uint32_t index, uint32_t offset, uint32_t size);

     /* Le pregunta a este peer si tiene una cierta pieza completa */
     bool havePiece(uint32_t index);

     /* Le avisa a este peer que tengo una pieza completa */
     void have(uint32_t index);

     /* Comienza el procesamiento y flujo de datos. */
     void start(std::string hash);

};


#endif /* PEER_H_INCLUDED */
