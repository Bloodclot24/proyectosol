#include "protocoloBitTorrent.h"

std::string ProtocoloBitTorrent::handshake(std::string str, std::string info_hash, 
                                           std::string peer_id) {

	char pstrlen= str.length();
	
//	std::cout << "length: " << length << std::endl;
//	
//	std::string pstrlen=  int32Astring(length);

	std::cout << "pstrlen: " << pstrlen << std::endl;
	
	
	std::string mensajeHandshake;
	uint64_t reserved= 0;//Revisar, xq deberia mostrar los 8bytes de 0's
	
	mensajeHandshake= pstrlen + str + int64Astring(reserved) + info_hash + peer_id;
	return mensajeHandshake;
}  
			
std::string ProtocoloBitTorrent::keepAlive() {

	uint32_t len= 0;
	len= this->bitStream.swap32ABigEndian(len);
	return int32Astring(len);
}
			
std::string ProtocoloBitTorrent::choke() {

	uint32_t len= 1;
	len= this->bitStream.swap32ABigEndian(len);
	char id = ID_CHOKE;
//	uint32_t id= ID_CHOKE;
//	id= this->bitStream.swap32ABigEndian(id);
	return int32Astring(len) + id;//int32Astring(id);
}
			
std::string ProtocoloBitTorrent::unchoke() {
	uint32_t len= 1;
	len= this->bitStream.swap32ABigEndian(len);
	char id= ID_UNCHOKE;
//	uint32_t id= ID_UNCHOKE;
//	id= this->bitStream.swap32ABigEndian(id);
	return int32Astring(len) + id;//int32Astring(id);
}
			
std::string ProtocoloBitTorrent::interested() {
	uint32_t len= 1;
	len= this->bitStream.swap32ABigEndian(len);
	char id= ID_INTERESTED;
//	uint32_t id= ID_INTERESTED;
//	id= this->bitStream.swap32ABigEndian(id);
	return int32Astring(len) + id;//int32Astring(id);
}
			
std::string ProtocoloBitTorrent::not_interested() {
	uint32_t len= 1;
	len= this->bitStream.swap32ABigEndian(len);
	char id = ID_NOT_INTERESTED;
//	uint32_t id= ID_NOT_INTERESTED;
//	id= this->bitStream.swap32ABigEndian(id);
	return int32Astring(len) + id;//int32Astring(id);
}
			
std::string ProtocoloBitTorrent::have(std::string piece) {
	uint32_t len= 5;
	len= this->bitStream.swap32ABigEndian(len);
	char id= ID_HAVE;
//	uint32_t id= ID_HAVE;
//	id = this->bitStream.swap32ABigEndian(id);
	return int32Astring(len) + /*int32Astring(id)*/ id + piece;
}

std::string ProtocoloBitTorrent::bitfield(std::string bitfield) {
	uint32_t len= 1 + bitfield.length();
	len= this->bitStream.swap32ABigEndian(len);
	char id = ID_BITFIELD;
//	uint32_t id = ID_BITFIELD;
//	id= this->bitStream.swap32ABigEndian(id);
	return int32Astring(len) + /*int32Astring(id)*/ id + bitfield;
}
					
std::string ProtocoloBitTorrent::request(uint32_t index, uint32_t begin, uint32_t length) {
	//length = 2^14(16KB)
	uint32_t len= 13;
	len= this->bitStream.swap32ABigEndian(len);
	char id = ID_REQUEST;
//	uint32_t id= ID_REQUEST;
//	id= this->bitStream.swap32ABigEndian(id);
	index= this->bitStream.swap32ABigEndian(index);
	begin= this->bitStream.swap32ABigEndian(begin);
	length= this->bitStream.swap32ABigEndian(length);
	return int32Astring(len) + /*int32Astring(id)*/ id +
	       int32Astring(index) + int32Astring(begin) +
	       int32Astring(length);
}
			
std::string ProtocoloBitTorrent::piece(uint32_t index, uint32_t begin, std::string block) {
	uint32_t len= 9 + block.length();
	len= this->bitStream.swap32ABigEndian(len);
	char id= ID_PIECE;
//	uint32_t id= ID_PIECE;
//	id= this->bitStream.swap32ABigEndian(id);
	index= this->bitStream.swap32ABigEndian(index);
	begin= this->bitStream.swap32ABigEndian(begin);
	return int32Astring(len) + /*int32Astring(id)*/ id + int32Astring(index) +
	       int32Astring(begin) + block;
}
			
std::string ProtocoloBitTorrent::cancel(uint32_t index, uint32_t begin, uint32_t length) {
	uint32_t len= 13; 
	len= this->bitStream.swap32ABigEndian(len);
	char id= ID_CANCEL;
//	uint32_t id= ID_CANCEL;
//	id = this->bitStream.swap32ABigEndian(id);
	index= this->bitStream.swap32ABigEndian(index);
	begin= this->bitStream.swap32ABigEndian(begin);
	length= this->bitStream.swap32ABigEndian(length);
	return int32Astring(len) + /*int32Astring(id)*/ id + int32Astring(index) +
	       int32Astring(begin) + int32Astring(length);
}
			
std::string ProtocoloBitTorrent::port(uint32_t listenPort) {
	uint32_t len= 3;
	len= this->bitStream.swap32ABigEndian(len);
	char id= ID_PORT;
//	uint32_t id= ID_PORT;
//	id= this->bitStream.swap32ABigEndian(id);
	listenPort= this->bitStream.swap32ABigEndian(listenPort);
	return  int32Astring(len) + /*int32Astring(id)*/ id + int32Astring(listenPort);
}

std::string ProtocoloBitTorrent::int64Astring(uint64_t valor){
	//convierto de int a string
	std::string snumero;
	std::stringstream cvz;
	cvz.width(8);
	cvz.fill('\0');
	cvz << std::hex << valor;
	snumero = cvz.str();
	return snumero;
}

std::string ProtocoloBitTorrent::int32Astring(uint32_t valor) {
	//convierto de int a string
	std::string snumero;
	std::stringstream cvz;
	cvz.width(4);
	cvz.fill('\0');
	cvz << std::hex << valor;
	snumero = cvz.str();
	return snumero;
}

/*--------------------------------------------------------------------------*/
Message* ProtocoloBitTorrent::decode(const char* mensaje) {
	
	std::string msg= mensaje;
	std::string auxiliar;
			
	Message* message= new Message();
	
	std::string length;
	length.assign(msg, 0, 4);
	const char* longitud = length.c_str();
	uint32_t* longitudMsj = (uint32_t*)longitud;
	uint32_t longMsj = this->bitStream.swap32ABigEndian(*longitudMsj);
	if(msg.length() > 4) {
	
		std::string idS;
		idS.assign(msg, 4, 1);
		
		int id= atoi(idS.c_str());
		
		if(id == ID_CHOKE) {
			std::cout << "choke" << std::endl;
			message->id= CHOKE;

		} else if(id == ID_UNCHOKE) {
			std::cout << "unchoke" << std::endl;
			message->id= UNCHOKE;
	
		} else if(id == ID_INTERESTED) {
			std::cout << "interested" << std::endl;
			message->id= INTERESTED;
	
		} else if(id == ID_NOT_INTERESTED) {
			std::cout << "not interested" << std::endl;
			message->id= NOT_INTERESTED;

		} else if(id == ID_HAVE) {
			std::cout << "have" << std::endl;
			message->id= HAVE;
			message->piece.assign(msg, 5,longMsj-5 );

		} else if(id == ID_BITFIELD) {
			std::cout << "bitfield" << std::endl;
			message->id= BITFIELD;
			//TODO ver tema de que bitfield es un string, ver si va
			//a seguir siendolo y hay que transformarlo en un int
			message->bitfield.assign(msg, 5, longMsj-5);

		} else if(id == ID_REQUEST) {
			std::cout << "request" << std::endl;
			message->id= REQUEST;
			auxiliar.assign(msg, 5, 4);
			const char* indiceAux = auxiliar.c_str();
			uint32_t* pIndice = (uint32_t*)indiceAux;
			uint32_t indice = this->bitStream.swap32ABigEndian(*pIndice);
			message->index= indice;
			auxiliar.assign(msg, 9, 4);
			const char* beginAux = auxiliar.c_str();
			uint32_t* pBegin = (uint32_t*)beginAux;
			uint32_t begin = this->bitStream.swap32ABigEndian(*pBegin);
			message->begin= begin;
			auxiliar.assign(msg, 13, longMsj-13);
			const char* lengthAux = auxiliar.c_str();
			uint32_t* pLength = (uint32_t*)lengthAux;
			uint32_t length = this->bitStream.swap32ABigEndian(*pLength);
			message->length= length;

		} else if(id == ID_PIECE) {
			std::cout << "piece" << std::endl;
			message->id= PIECE;
			auxiliar.assign(msg, 5, 4);
			const char* indiceAux = auxiliar.c_str();
			uint32_t* pIndice = (uint32_t*)indiceAux;
			uint32_t indice = this->bitStream.swap32ABigEndian(*pIndice);
			message->index= indice;
			auxiliar.assign(msg, 9, 4);
			const char* beginAux = auxiliar.c_str();
			uint32_t* pBegin = (uint32_t*)beginAux;
			uint32_t begin = this->bitStream.swap32ABigEndian(*pBegin);
			message->begin= begin;
			auxiliar.assign(msg, 13, longMsj-13);
			//TODO ver tema de block, xq es un string, ver si va a seguir siendolo
			//y hay que convertir a int o si va a ser un int directamente
			message->block= atoi(auxiliar.c_str());
	
		} else if(id == ID_CANCEL) {
			std::cout << "cancel" << std::endl;
			message->id= CANCEL;
			auxiliar.assign(msg, 5, 4);
			const char* indiceAux = auxiliar.c_str();
			uint32_t* pIndice = (uint32_t*)indiceAux;
			uint32_t indice = this->bitStream.swap32ABigEndian(*pIndice);
			message->index= indice;
			auxiliar.assign(msg, 9, 4);
			const char* beginAux = auxiliar.c_str();
			uint32_t* pBegin = (uint32_t*)beginAux;
			uint32_t begin = this->bitStream.swap32ABigEndian(*pBegin);
			message->begin= begin;
			auxiliar.assign(msg, 13, longMsj-13);
			const char* lengthAux = auxiliar.c_str();
			uint32_t* pLength = (uint32_t*)lengthAux;
			uint32_t length = this->bitStream.swap32ABigEndian(*pLength);
			message->length= length;

		} else if(id == ID_PORT) {
			std::cout << "port" << std::endl;
			message->id= PORT;
			auxiliar.assign(msg, 5,4);
			const char* portAux = auxiliar.c_str();
			uint32_t* pPort = (uint32_t*)portAux;
			uint32_t port = this->bitStream.swap32ABigEndian(*pPort);
			message->listenPort= port;
			
		}
	} else 
		message->id= KEEP_ALIVE;
	
	return message;
}

