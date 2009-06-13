#ifndef PEER_H_INCLUDED
#define PEER_H_INCLUDED

class Torrent;

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
     /* Dado un peer en forma compacta (ip.ip.ip.ip:puerto), crea un
      * nuevo peer */
     Peer(const std::string& host, int puerto, Torrent* torrent);

     /* Devuelve un bitField con las piezas del peer */
     const BitField* getBitField();

     /* Comienza el procesamiento y flujo de datos. */
     void start(std::string hash);


};


#endif /* PEER_H_INCLUDED */
