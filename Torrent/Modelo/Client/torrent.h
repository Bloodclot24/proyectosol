#ifndef TORRENT_H_INCLUDED
#define TORRENT_H_INCLUDED


class Peer;
/** 
 * Estados posibles del Torrent
 */
enum EstadoTorrent {STOPPING, STOPPED, PAUSED, STARTING, DOWNLOADING, SEEDING, ERROR};
class Torrent;

#include "socket.h"
#include "threads.h"
#include "threadEmisor.h"
#include "threadReceptor.h"
#include "bitField.h"
#include "downloadSlot.h"

#include "../ParserBencode/parserBencode.h"
#include "../HTTP/HttpRequest.h"
#include "../HTTP/HttpResponse.h"
#include "../SHA1/sha1.h"
#include "mensaje.h"
#include "client.h"

#include "torrentFile.h"
#include "peer.h"
#include "threads.h"

#include "../../Controlador/controlador.h"

#include <iostream>
#include <limits.h>
#include <math.h>
#include <queue>
#include "deque.h"

#define MAX_CONNECTED_PEERS 20
#define REQ_HASH "info_hash"
#define REQ_PEER_ID "peer_id"
#define REQ_PORT "port"
#define REQ_UPLOADED "uploaded"
#define REQ_DOWNLOADED "downloaded"
#define REQ_CORRUPT "corrupt"
#define REQ_LEFT "left"
#define REQ_COMPACT "compact"
#define REQ_NUMWANT "numwant"

/** 
 * Clase que modela cada Torrent que maneja el cliente.
 */
class Torrent: public Thread{
private:
	 Client* client;

     std::string nombreTorrent; /**< El nombre del archivo .torrent */

     std::list<TorrentFile*>* archivos; /**< La lista de archivos del
					 * torrent */

     uint32_t sizeInPieces;	/**< Cantidad de piezas total del
				 * Torrent */

     uint32_t pieceSize;	/**< El tamao de cada pieza del
				 * Torrent */

     std::string announceUrl;	/**< URL del announce del tracker. */

     std::list<std::string> announceUrlList; /**< Lista de URL's para
					      * realizar el announce
					      * del Tracker. Si esta
					      * vacia, significa que
					      * se debe utilizar
					      * @announceUrl. */

     int creationDate;		/**< Fecha de creacion del .torrent */
     
     std::string comment;	/**< Comentario del .torrent */

     std::string createdBy;	/**< Creador del .torrent */

     int encoding;		/**< Codificacion del .torrent */

     std::string idHash;	/**< Hash que identifica al torrent
				 * frente al tracker */

     EstadoTorrent estado;	/**< Estado actual del Torrent. */
     Mutex mutexEstado;		/**< Mutex utilizado para bloquear el
				 * estado del Torrent */

     Socket* socket;
     ThreadReceptor *receptor;
     ThreadEmisor *emisor;

     bool valido;		/**< Indica si el Torrent es
				 * valido. Se utiliza para saber si
				 * ocurrio algun error en el
				 * constructor. */
     
     std::list<Peer*> listaPeers; /**< Lista de Peers disponibles para
				   * conectar */
     std::list<Peer*> listaPeersConectados; /**< Lista de peers
					     * conectados
					     * actualmente */

     std::list<Peer*> listaPeersActivos; /**< Lista de peers
					  * conectados pero que nos
					  * estan enviando datos
					  * actualmente */

     std::list<Peer*> listaPeersEspera;	/**< Lista de peers conectados
					 * que acaban de terminar una
					 * transferencia */
     
     Mutex mutexPeers;		/**< Bloquea la lista de Peers */

     Mutex mutexBitField;	/**< Mutex para el bitField */
     BitField *bitField;	/**< BitField que representa las
				 * piezas (validas) que tenemos */


     /**< representa las piezas que estamos bajando. Como cada pieza
      * se baja por partes, esto nos indica que partes de cada pieza
      * tenemos y que partes nos faltan */     
     Deque<DownloadSlot*> piezasEnProceso;	

     Mutex mutexPiezasAVerificar;
     
     std::map<int,int> piezasAVerificar; /**< Map que contiene las
					  * piezas que todavia no
					  * fueron verificadas. Cada
					  * par es del tipo <numero de
					  * pieza, bloques>. Cuando
					  * recibimos todas los
					  * bloques de esa pieza
					  * realizamos la verificacion
					  * de la misma. */

     
     std::list<uint32_t> listaPiezasAVerificar;	/**< Lista con las
						 * piezas que deben
						 * ser verificadas */
     std::queue<uint32_t> colaPiezasAAnunciar; /**< Cola con las
						* piezas que
						* deboanunciar */
     std::list<DownloadSlot*> listaPiezasPedidas; /**< Piezas que
					      * fueron asignadas a
					      * peers */
     

     int partsRequested;	/**< indica la cantidad de partes que
				 * estamos esperando en este momento */

     int peersActivos;		/**< Cantidad de peers activos */

     Mutex requestMutex;	/**< Mutex para bloquear la condicion
				 * de request. */
     CVariable requestCondition; /**< CVariable para esperar la
				  * condicion de request. */

     
     Deque<Peer*> peersEnEspera; /**< Cola con los peers que nos
				  * enviaron un Unchoke y estan
				  * disponibles para realizar
				  * requests */

     uint32_t announceTime;	/**< intervalo minimo entre
				 * announce */
     uint32_t timeToAnnounce;	/**< Tiempo restante para el proximo
				 * announce */

     Mutex downloadMutex;

     Controlador *controlador;	/**< El controlador mediante el cual
				 * me comunico con la vista. */

     uint32_t piezasVerificadas; /**< Cantidad de piezas verificadas
				  * y validas. */

     std::list<std::string> listaDireccionesPeers; /**< Lista que se le
						  * entrega al
						  * controlador para
						  * que muestre en la
						  * vista */
     Mutex mutexListaDireccionsPeers;
          
     uint32_t velSubida;
     uint32_t velBajada;

     uint32_t downloaded, uploaded, corrupt, left;

private:

     /** 
      * Devuelve el numero de pieza que es mas dificil de conseguir
      * (de las que no tenemos).
      * @return El numero de la pieza.
      */
     uint32_t rarestFirst();

     /** 
      * Armala lista de trackers. Dada una lista de listas de strings
      * codificado en bencode, arma la lista de trackers y la almacena
      * en @announceUrlList
      * 
      * @param listaLista La lista codificada.
      *
      * @see ParserBencode
      */
     void armarListaDeTrackers(const std::list<BeNode*> &listaLista);

     /** 
      * Rota la lista de trackers. Cuando nos intentamos conectar a un
      * tracker y fallamos, rotamos la lista de trackers e intentamos
      * con el siguiente.
      */
     void rotarTrackers();

     /** 
      * Devuelve el estado del torrent.
      * @return El estado actual del Torrent.
      */
     EstadoTorrent getEstado() { 
	  Lock lock(mutexEstado);
	  return estado; 
     };

public:
     /** 
      * Crea un nuevo Torrent. Dado el nombre de un archivo '.torrent'
      * y opcionalmente un BitField, crea un Torrent.
      * 
      * @param fileName El nombre del archivo '.torrent'
      * @param client El cliente actual.
      *
      * @param bitfieldGuardado El BitField con la informacion de las
      * piezas del Torrent. Si es NULL o no se especifica, se crea un
      * nuevo BitField. En este ultimo caso, si alguno de los archivos
      * contenidos en el '.torrent' ya existian en disco, se realiza
      * una verificacion de cada una de las piezas.
      */
     Torrent(const char* fileName, Client* client,
             BitField* bitfieldGuardado= NULL);

     /** 
      * Comienza el procesamiento del Torrent.
      * 
      * @return Si tuvo exito, retorna 1, si no, 0;
      */
     int start();

     /** 
      * Devuelve el tamao en bytes total de todos los archivos
      * contenidos en el torrent.
      *
      * @return Tamao en bytes.
      */
     uint64_t getTotalSize();

     /** 
      * Devuelve el nombre del '.torrent' original.
      * 
      * @return Nombre del '.torrent'
      */
     const std::string& getName(){ return nombreTorrent; }

     /** 
      * Indica si el objeto es valido o no. Se usa para saber si hubo
      * o no algun problema en el constructor.
      * 
      * @return true si es valido, false en caso contrario.
      */
     bool isValid();

     /** 
      * Devuelve una copia del BitField asociado al Torrent.
      * 
      * @return El BitField con las piezas completas del Torrent.
      */
     BitField getBitField();

     /** 
      * Agrega un Peer a la lista de Peers, cuidando de que no este
      * repetido.
      * 
      * @param peer Un puntero al Peer a agregar.
      */
     void agregarPeer(Peer* peer);
     
     /** 
      * Rutina principal del torrent. Aqui se maneja la logica.
      * @see Thread
      */
     virtual void run();

     /** 
      * Dado un indice de una pieza, obtiene el FileTorrent al que
      * corresponde.
      * 
      * @param index El indice de la pieza.
      * 
      * @return Un puntero a TorrentFile o NULL si el @index es
      * invalido.
      */
     TorrentFile* obtenerArchivo(uint32_t index);

     /** 
      * Dado un indice devuelve el offset dentro del archivo donde cae
      * esa pieza.
      * 
      * @param index El indice de la pieza.
      * 
      * @return El offset dentro del archivo.
      */
     uint64_t obtenerByteOffset(uint32_t index);

     /** 
      * Escribe un bloque de datos en el/los archivo/archivos que
      * corresponda.
      * 
      * @param data Un puntero a los datos a escribir.
      *
      * @param index El numero de pieza al que corresponden los datos.
      *
      * @param offset El offset dentro de la pieza donde se deben
      * escribir los datos.
      *
      * @param size El temao del buffer de datos.
      * 
      * @return Si tuvo exito devuelve 0, en caso contrario devuelve -1.
      */
     int writeData(const char* data, uint32_t index, uint32_t offset, uint32_t size);

     /** 
      * Lee un bloque de datos de el/los archivo/archivos que
      * corresponda.
      * 
      * @param data Un buffer para almacenar los datos.
      *
      * @param index El indice de la pieza de la cual se leen los datos.
      *
      * @param offset El offset dentro de la pieza.
      *
      * @param size El tamao del buffer de datos.
      * 
      * @return Devuelve la cantidad de bytes leidos si tuvo exito o
      * -1 en caso contrario.
      */
     uint32_t readData(char* data, uint32_t index, uint32_t offset, uint32_t size);

     /** 
      * Metodo que llaman los peers cuando se conectan exitosamente.
      * 
      * @param peer El peer que llamo al metodo.
      */
     void peerConected(Peer *peer);

     /** 
      * Metodo que llaman los peers cuando envian 'choke'.
      * 
      * @param peer El Peer que nos envio el 'choke'
      *
      * @see ProtocoloBitTorrent
      */
     void peerChoked(Peer* peer);

     /** 
      * Metodo que llaman los peers cuando envian 'unchoke'.
      * 
      * @param peer El Peer que nos envio el 'unchoke'
      *
      * @see ProtocoloBitTorrent
      */
     void peerUnchoked(Peer* peer);

     /** 
      * Metodo que llaman los Peers cuando terminan la transferencia
      * de un bloque.
      * 
      * @param peer El peer que llamo al metodo.
      *
      * @param ds El DownloadSlot asociado al bloque transferido.
      */
     void peerTransferFinished(Peer* peer, DownloadSlot *ds);

     /** 
      * Metodo que llaman los Peers cuando cancelan la transferencia
      * de un bloque.
      * 
      * @param peer El peer que llamo al metodo.
      *
      * @param ds El DownloadSlot asociado al bloque transferido.
      */
     void peerTransferCanceled(Peer* peer, DownloadSlot * ds);

     /** 
      * Aborta un request, insertandolo nuevamente en la cola de
      * espera.
      * 
      * @param ds El slot a reinsertar en la lista de pendientes.
      */
     void abortRequest(DownloadSlot* ds);

     /** 
      * Avisa a todos los peers conectados que tenemos una pieza.
      * 
      * @param index El numero de pieza a anunciar.
      */
     void anunciarPieza(uint32_t index);

     /** 
      * Realiza un announce al primer tracker de la lista.
      * 
      * @return Si tuvo exito devuelve 0, si falla devuelve -1.
      */
     int announce();

     /** 
      * Recorre la lista de trackers anunciando el torrent hasta que
      * alguno responda satisfactoriamente. Si no se pudo anunciar a
      * ninguno exitosamente, devuelve un numero menor a 0.
      * 
      * @return Codigo de error. Menor a cero si hubo algun
      * error. Cero o mayor si se pudo comunicar con algun tracker.
      */     
     int do_announce();
     
     /** 
      * Si hay que verificar alguna pieza, la verifica. Si la pieza es
      * correcta o no, se ve reflejado en el bitfield.  Adicionalmente
      * inserta los numeros de pieza en la lista de piezas a anunciar.
      * 
      */
     void verificarPiezasPendientes();

     /** 
      * Anuncia a todos los peers (conectados, activos y en espera)
      * las piezas que tengo pendientes de anunciar.
      * 
      */
     void anunciarPiezasPendientes();

     /** 
      * Realiza un scrape al primer tracker de la lista.
      * 
      * @return  Si tuvo exito devuelve 0, en caso contrario -1.
      */
     int scrape();

     /** 
      * Indica si una pieza es o no valida.
      * 
      * @param index El numero de la pieza.
      * 
      * @return Si es valida, 1, si es invalida, 0. Si el numero de
      * pieza en invalida -1 y si ocurre algun otro error, -2.
      */
     int validarPieza(uint32_t index);

     /** 
      * Detiene al Torrent. Si hay descargas o subidas en proceso, las
      * cancela y si hay Peers conectados los desconecta.
      * 
      * @return 1 si tuvo exito.
      */
     int stop();

     /** 
      * Pausa al Torrent. Si hay descargas o subidas en proceso, no
      * las detiene, espera a que finalizen, pero no se aceptan nuevas
      * descargas ni subidas.
      * 
      * @return 1 si tuvo exito.
      */
     int pause();
	

     /** 
      * Devuelve el URL del primer tracker en la lista de trackers.
      * 
      * @return La direccion del tracker.
      */
     const std::string getAnnounceUrl() { return announceUrl; };
	 
     /** 
      * Devuelve la cantidad de peers activos.
      * @return La cantidad de peers activos.
      */
     const int getPeersActivos() { return peersActivos; };
	 
     /** 
      * Asigna un controlador al Torrent, para comunicarse con la
      * vista.
      * 
      * @param controlador El controlador.
      */
     void setControlador(Controlador *controlador){ 
	 	this->controlador = controlador;
     }
      
     const std::list<std::string>* getAnnounceUrlList() { return &announceUrlList; }
     
     /** 
      * Devuelve el porcentaje bajado del total del Torrent.
      *
      * @return Elporcentaje bajado.
      */
     double getPorcentaje();
 
     /** 
      * Devuelve la velocidad de subida en bytes/s.
      * 
      * @return La velocidad de subida.
      */
     uint32_t getVelocidadSubida();

     /** 
      * Devuelve la velocidad de bajada en bytes/s.
      * 
      * @return La velocidad de bajada.
      */
     uint32_t getVelocidadBajada();

     /** 
      * Devuelve la cantidad de piezas que conforman al torrent.
      * 
      * @return Cantidad totales de piezas.
      */
     uint32_t getSizeInPieces(){
	  return sizeInPieces;
     }

     /** 
      * Devuelve el tamao de cada pieza del torrent, excepto por la
      * ultima.
      * 
      * @return Tamao de las piezas.
      */
     uint32_t getPieceSize(){
	  return pieceSize;
     }

     uint32_t getLastPieceSize(){
	  return getTotalSize()%getPieceSize();
     }

     /** 
      * Devuelve el estado del torrent.
      * @return El estado actual del Torrent.
      */
     EstadoTorrent getState() { 
	  return estado; 
     };


     /** 
      * Devuelve una lista con las ips de los peers
      * con los que el torrent esta conectado.
      * 
      * 
      * @return La lista de ip de peers.
      */
     std::list<std::string> getListaPeers();     

     void cleanup();
     
     /** 
      * Destruye al Torrent y libera los recursos asociados.
      * Llama a stop().
      */
     ~Torrent();
};

#endif /* TORRENT_H_INCLUDED */
