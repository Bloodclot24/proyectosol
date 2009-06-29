#ifndef PROTOCOLOBITTORRENT_H_
#define PROTOCOLOBITTORRENT_H_

#include "../Client/deque.h"
#include "../SHA1/bitStream.h"
#include <iostream>
#include <sstream>
#include <list>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

#define ID_CHOKE 0
#define ID_UNCHOKE 1
#define ID_INTERESTED 2 
#define ID_NOT_INTERESTED 3
#define ID_HAVE 4
#define ID_BITFIELD 5
#define ID_REQUEST 6
#define ID_PIECE 7
#define ID_CANCEL 8
#define ID_PORT 9

enum MsgId {CHOKE, UNCHOKE, INTERESTED, NOT_INTERESTED, HAVE, BITFIELD,
	        REQUEST, PIECE, CANCEL, PORT, KEEP_ALIVE};

typedef struct {
	MsgId id;
	std::string bitfield;
	uint32_t index;
	uint32_t begin;
	uint32_t length;
	std::string block;
	uint32_t listenPort;
} Message;

/****************************************************************************/
/**
 * Protocolo de mensajes utilizados para la comunicacion entre peers.
 */ 
class ProtocoloBitTorrent {
	
	private:
		BitStream bitStream;
		std::string int64Astring(uint64_t valor);
		std::string int32Astring(uint32_t valor);

	public:
		/**
		 * Crea un protocolo BitTorrent. 
		 */
		ProtocoloBitTorrent(){}
		
		/**
		 * Construye el mensaje handshake. 
		 * 
		 * @param str Cadena que identifica el protocolo. 
		 * @param info_hash Cadena 20-byte SHA1 hash de la metainfo file.
		 * @param peer_id Cadena 20-byte de la ID unica del cliente.
		 * 
		 * @return El mensaje handshake.
		 */	
		std::string handshake(std::string str, std::string info_hash, 
		                      std::string peer_id);

		/**
		 * Construye el mensaje keep-alive. 
		 *
		 * @return El mensaje keep-alive.
		 */	
		std::string keepAlive();

		/**
		 * Construye el mensaje choke. 
		 * 
		 * @return El mensaje choke.
		 */	
		std::string choke();
		
		/**
		 * Construye el mensaje unchoke. 
		 * 
		 * @return El mensaje unchoke.
		 */		
		std::string unchoke();
			
		/**
		 * Construye el mensaje interested. 
		 * 
		 * @return El mensaje interested.
		 */		
		std::string interested();
		
		/**
		 * Construye el mensaje not interested. 
		 * 
		 * @return El mensaje not interested.
		 */		
		std::string not_interested();
		
		/**
		 * Construye el mensaje have. 
		 * 
		 * @param piece El numero de pieza que se quiere anunciar.
		 * 
		 * @return El mensaje have.
		 */		
		std::string have(uint32_t piece);
			
		/**
		 * Construye el mensaje bitfield. 
		 * 
		 * @param length La longitud del bitfield. 
		 * 
		 * @return El mensaje bitfield.
		 */		
		std::string bitfield(uint32_t length);
		
		/**
		 * Construye el mensaje request. 
		 * 
		 * @param index El numero de pieza.
		 * @param begin Offset en byte de la ubicacion del bloque en la pieza.
		 * @param length La longitud del bloque de datos.
		 * 
		 * @return El mensaje request.
		 */				
		std::string request(uint32_t index, uint32_t begin, uint32_t length);
		
		/**
		 * Construye el mensaje piece. 
		 * 
		 * @param index El numero de pieza.
		 * @param begin Offset en byte de la ubicacion del bloque en la pieza.
		 * @param length La longitud del bloque de datos.
		 * 
		 * @return El mensaje piece.
		 */		
		std::string piece(uint32_t index, uint32_t begin, uint32_t length);
			
		/**
		 * Construye el mensaje cancel. 
		 * 
		 * @param index El numero de pieza.
		 * @param begin Offset en byte de la ubicacion del bloque en la pieza.
		 * @param length La longitud del bloque de datos.
		 * 
		 * @return El mensaje cancel.
		 */		
		std::string cancel(uint32_t index, uint32_t begin, uint32_t length);
		
		/**
		 * Construye el mensaje port. 
		 * 
		 * @param listenPort Puerto donde el peer escuchara.
		 * 
		 * @return El mensaje port.
		 */	
		std::string port(uint32_t listenPort);
			
		/**
		 * Dado un mensaje lo decodifica.
		 * 
		 * @param deque Cola de donde salen los datos a interpretar. 
		 * 
		 * @return El mensaje con los datos decodificados.
		 */		
		Message* decode(Deque<char> &deque);	
};

/****************************************************************************/
#endif /*PROTOCOLOBITTORRENT_H_*/
