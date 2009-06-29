#ifndef PEER_H_INCLUDED
#define PEER_H_INCLUDED

class Torrent;

#define REQUEST_SIZE_DEFAULT 16*1024

#include "socket.h"
#include "../ProtocoloBitTorrent/protocoloBitTorrent.h"
#include "threads.h"
#include "torrent.h"
#include "bitField.h"

#include "downloadSlot.h"
#include "deque.h"

/** 
 * Clase que representa al Peer remoto.
 * 
 */
class Peer:public Thread{
private:
     Socket *socket;
     ThreadEmisor *emisor;
     ThreadReceptor *receptor;
     
     Torrent* torrent;		/**< El Torrent asociado al Peer */

     int am_choking, am_interested; /**< Indican si estamos o no
				     * interesados en el Peer y si le
				     * enviamos o no el mensaje
				     * 'Choke'. */

     int peer_choking, peer_interested; /**< Indica si el Peer esta o
					 * no interesado en nosotros y
					 * si nos envio o no el
					 * mensaje de 'Choke'. */

     std::string hash;		/**< hash recibido en el 'handshake' */


     BitField *bitField;	/**< BitField con las partes que posee
				 * este Peer. */

     std::string name;		/**< Nombre del Peer */
     uint32_t port;		/**< El puerto destino */
     
     bool entrante;		/**< Indica si el Peer es entrante o
				 * no. Un Peer es entrante cuando se
				 * inicia la conexion externamente (el
				 * Peer nos contacta a nosotros y no a
				 * la inversa). */

     Deque<DownloadSlot*> requests; /**< Cola donde se guardan
				     * los DownloadSlots que
				     * recibimos del Torrent
				     * cuando hacemos un
				     * request. */

     uint64_t downloaded;	/**< Cantidad de bytes que bajo este
				 * peer */
     uint64_t uploaded;		/**< Cantidad de bytes que subio el
				 * peer. */

     uint64_t corrupted;	/**< Cantidad de piezas corruptas que
				 * nos envio */
protected:
     /** 
      * Metodo principal de la clase. Toda la logica se desarrolla aca
      * dentro.
      * 
      * @see Thread
      */
     virtual void run();
     
public:
     bool conectado;

     /** 
      * Dado un peer en forma compacta (ip.ip.ip.ip:puerto), crea un nuevo peer.
      * 
      * @param host La direccion del peer en la forma ipv4.
      * @param puerto El puerto del peer.
      * @param torrent El torrent con el que esta asociado el peer.
      * 
      */
     Peer(const std::string& host, int puerto, Torrent* torrent);
	
     /** 
      * Crea un peer con el socket dado.
      * 
      * @param socket Socket atraves del cual nos comunicamos con el
      * peer.
      * 
      * @see Socket
      */
     Peer(Socket* socket);
	 
     /** 
      * Devuelve un bitField con las piezas que provee este peer.
      * @return El bitField del peer.
      */
     const BitField* getBitField();

     /** 
      * Devuelve el nombre del peer.
      * 
      * @return El nombre del peer.
      */
     const std::string& getName();

     /** 
      * Devuelve el thread emisor del peer. Utilizando este
      * ThreadEmisor, podemos enviar datos al peer remoto.
      * 
      * @return Un threadEmisor.
      *
      * @see ThreadEmisor
      */
     ThreadEmisor* getEmisor();

     /** 
      * Envia el mensaje de 'Interested' o 'NotInterested' al peer.
      * Adicionalmente, modifica el estado de am_interested.
      * 
      * @param bool Indica si estamos o no interesados en el peer. Si
      * es true, se envia el mensaje 'Interested', en caso contrario,
      * se envia el mensaje 'NotInterested'.
      * 
      * @see ProtocoloBittorrent
      */
     void setInterested(bool);
     
     /** 
      * Obtiene el estado de am_interested.  
      *
      * @return true si estamos interesados en el Peer, false si no.
      */
     bool getInterested();

     /** 
      * Envia el mensaje de 'Choke' o 'Unchoke' al peer.
      * Adicionalmente, modifica el estado de am_choking.
      * 
      * @param bool Si es true, se envia el mensaje 'Choke', en caso
      * contrario, se envia el mensaje 'Unchoke'.
      * 
      * @see ProtocoloBittorrent
      */
     void setChoke(bool);

     /** 
      * Obtiene el estado de am_choking.  
      *
      * @return true si ya enviamos 'Choke' al Peer, false si no.
      */
     bool getChoke();

     /* Envia un request de una parte */
     void sendRequest(uint32_t index, uint32_t offset, uint32_t size);

     /** 
      * Inicia un Request al Peer.
      * 
      * @param ds El downloadSlot con la informacion del bloque a
      * pedir.
      *
      * @see DownloadSlot
      */
     void sendRequest(DownloadSlot* ds);

     /** 
      * Le pregunta a este peer si tiene una cierta pieza completa.
      * 
      * @param index El numero de pieza.
      * 
      * @return Si el Peer posee la pieza completa true, si no, false.
      */
     bool havePiece(uint32_t index);

     /** 
      * Le avisa a este peer que tengo una pieza completa.
      * 
      * @param index El numero de pieza a anunciar.
      *
      * @see ProtocoloBittorrent
      */
     void have(uint32_t index);

     /** 
      * Comienza el procesamiento y flujo de datos.
      * 
      * @param hash El hash que identifica al Torrent.
      */
     void start(std::string hash);

     /** 
      * Finaliza el thread y libera memoria.
      * 
      * @see Thread
      */
     void finish();

};


#endif /* PEER_H_INCLUDED */
