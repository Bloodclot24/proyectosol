#ifndef PROTOCOLOBITTORRENT_H_
#define PROTOCOLOBITTORRENT_H_
#include "../Client/deque.h"
#include "../SHA1/bitStream.h"
#include <iostream>
#include <sstream>
#include <list>
#include <stdlib.h>

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

class ProtocoloBitTorrent
{
	private:
			BitStream bitStream;
			std::string int64Astring(uint64_t valor);
			std::string int32Astring(uint32_t valor);

	public:
			ProtocoloBitTorrent(){}
			
			std::string handshake(std::string str, std::string info_hash, 
			                      std::string peer_id);
			
			std::string keepAlive();
			
			std::string choke();
			
			std::string unchoke();
			
			std::string interested();
			
			std::string not_interested();
			
			std::string have(uint32_t piece);
			
			std::string bitfield(uint32_t length);
					
			std::string request(uint32_t index, uint32_t begin, uint32_t length);
			
			std::string piece(uint32_t index, uint32_t begin, uint32_t length);
			
			std::string cancel(uint32_t index, uint32_t begin, uint32_t length);
			
			std::string port(uint32_t listenPort);
			
			Message* decode(Deque<char> &deque);	
};

#endif /*PROTOCOLOBITTORRENT_H_*/
