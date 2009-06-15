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

protected:
     void run();
     
public:
     bool conectado;
     /* Dado un peer en forma compacta (ip.ip.ip.ip:puerto), crea un
      * nuevo peer */
     Peer(const std::string& host, int puerto, Torrent* torrent);

     /* Devuelve un bitField con las piezas del peer */
     const BitField* getBitField();

     ThreadEmisor* getEmisor();

     void setInterested(bool);
     bool getInterested();
     void setChoke(bool);
     bool getChoke();

     void sendRequest(uint32_t index, uint32_t offset, uint32_t size);

     bool havePiece(uint32_t index);

     /* Comienza el procesamiento y flujo de datos. */
     void start(std::string hash);


};


#endif /* PEER_H_INCLUDED */
