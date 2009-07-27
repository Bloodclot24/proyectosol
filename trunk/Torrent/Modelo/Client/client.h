#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include "torrent.h"
#include <iostream>
#include <sstream>
#include <list>
#include <string.h>
#include "../ParserBencode/parserBencode.h"
#include "threadAceptor.h"
#include "socket.h"

class Client;
#include "../../Controlador/controlador.h"

#define CLIENT_ID      "-SN010-abcdefghijklm"
#define PORT_IN        12345
#define PEERS_NUM_WANT 50
#define MAX_REQUESTS   25

/**
 * Modelo del cliente torrent
 */
class Client {
private:
     std::string nickUsuario;
     std::list<Torrent*> torrents;
     Controlador* controlador;

     Socket* socket;
     ThreadAceptor* threadAceptor;

     uint32_t puertoPorDefecto;
     uint32_t cantidadDePeers;

     std::map<std::string,std::string> trackersIds; /**< Lista Ids de los trackers.*/

     Torrent* buscarTorrent(const char* filename);


public:

	Client(Controlador* controlador);

     /**
      * Agrega un torrent.
      *
      * @param path Ruta del archivo .torrent.
      * @param bitfieldGuardado El BitField con la informacion de las
      * piezas del Torrent. Si es NULL o no se especifica, se crea un
      * nuevo BitField. En este ultimo caso, si alguno de los archivos
      * contenidos en el '.torrent' ya existian en disco, se realiza
      * una verificacion de cada una de las piezas.
      */
    bool addTorrent(const char* path, BitField* bitfield= NULL);

     /**
      * Comienza a descargar un torrent.
      *
      * @param filename Nombre del archivo torrent.
      */
    bool start(const char* filename);

	/**
	 * Detiene la descarga de un torrent.
	 *
     * @param filename Nombre del archivo torrent.
	 */
    bool stop(const char* filename);

	/**
	 * Suspende la descarga de un torrent.
	 *
     * @param filename Nombre del archivo torrent.
	 */
    bool pause(const char* filename);

	/**
	 * Borra la descarga de un torrent.
	 *
     * @param filename Nombre del archivo torrent.
	 */
    bool remove(const char* filename);

	/**
	 * Obtiene la lista de torrents.
	 *
     * @return La lista de torrent.
	 */
	const std::list<Torrent*>* getListaTorrents();

	/**
	 * Obtiene el puerto de conexion por defecto.
	 *
	 * @return El puerto de conexion por defecto.
	 */
	uint32_t  getPortDefault();

	/**
	 * Modifica el puerto de conexion por defecto.
	 *
	 * @param puerto El puerto de conexion por defecto a setear.
	 */
	void setPortDefault(uint32_t puerto);

	/**
	 * Obtiene el numero de peers para un torrent.
	 *
	 * @return El numero de peers para un torrent.
	 */
	uint32_t getNumPeersForTorrent();

	/**
	 * Modifica el numero de peers para un torrent.
	 *
	 * @param numeroDePeers El numero de peers para un torrent a setear.
	 */
	void setNumPeersForTorrent(uint32_t numeroDePeers);

	/**
	 * Informa si el .torrent ya esta descargando.
	 *
     * @param filename Nombre del archivo torrent.
     *
     * @return Devuelve true si el archivo ya existe, false caso contrario.
	 */
	bool existeTorrent(const char* filename);

	/**
	 * Informa si el ya se ha establecido una conexion con el tracker.
	 *
     * @param tracker Nombre del tracker.
     *
     * @return Devuelve true si el tracker ya existe, false caso contrario.
	 */
	bool existeTracker(const std::string tracker);

	/**
	 * Informa si el ya se ha establecido una conexion con el tracker.
	 *
     * @param tracker Nombre del tracker.
     *
     * @return Devuelve la id del tracker.
	 */
	const std::string trackerId(const std::string tracker);

	/**
	 * Agrega un tracker con su respectiva id.
	 *
     * @param tracker Nombre del tracker.
     * @param id Id del tracker.
	 */
	void addTracker(const std::string tracker, const std::string id);

	/**
	 * Modifica la id del tracker.
	 *
     * @param tracker Nombre del tracker.
     * @param id Id del tracker.
	 */
	void modificarIdTracker(const std::string tracker, const std::string id);

	/**
	 * Devuelve el id del cliente.
	 *
     * @return Id del cliente.
     *
	 */
	const std::string getClientId();

	/**
	 * Devuelve el puerto de entrada.
	 *
     * @return puerto de entrada.
     *
	 */
	const std::string getPortIn();

	/**
	 * Devuelve la cantidad de peers a pedir.
	 *
     * @return cantidad de peers a pedir.
     *
	 */
	const std::string getPeersNumWant();

	/**
	 * Devuelve la cantidad maxima de request.
	 *
     * @return cantidad de request.
     *
	 */
	const std::string getMaxRequest();

	/**
	 * Convierte de int a string.
	 *
	 * @return el numero convertido en string.
	 *
	*/
	std::string intAstring(uint32_t valor) {
	     	//convierto de int a string
	     	std::string snumero;
	     	std::stringstream cvz;
	     	cvz << valor;
	     	snumero = cvz.str();
	     	return snumero;
	}

	/**
	* Dado un hash obtiene el torrent correspondiente al mismo.
	* @return el puntero al torrent correspondiente o NULL en
	* caso de no encontrarlo.
    */
	Torrent* buscarPorHash(std::string hash);

	/**
     * Destruye el cliente y libera todos los torrents.
     */
    ~Client();
};

#endif /* CLIENT_H_INCLUDED */
class Client;
