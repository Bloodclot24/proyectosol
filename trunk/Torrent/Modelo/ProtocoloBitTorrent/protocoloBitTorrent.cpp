#include "protocoloBitTorrent.h"

std::string ProtocoloBitTorrent::handshake(std::string str, std::string info_hash, 
                                           std::string peer_id) {

	int length= str.length();
	
	std::cout << "length: " << length << std::endl;
	
	std::string pstrlen=  int32Astring(length);

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
	uint32_t id= ID_CHOKE;
	id= this->bitStream.swap32ABigEndian(id);
	return int32Astring(len) + int32Astring(id);
}
			
std::string ProtocoloBitTorrent::unchoke() {
	uint32_t len= 1;
	len= this->bitStream.swap32ABigEndian(len);
	uint32_t id= ID_UNCHOKE;
	id= this->bitStream.swap32ABigEndian(id);
	return int32Astring(len) + int32Astring(id);
}
			
std::string ProtocoloBitTorrent::interested() {
	uint32_t len= 1;
	len= this->bitStream.swap32ABigEndian(len);
	uint32_t id= ID_INTERESTED;
	id= this->bitStream.swap32ABigEndian(id);
	return int32Astring(len) + int32Astring(id);
}
			
std::string ProtocoloBitTorrent::not_interested() {
	uint32_t len= 1;
	len= this->bitStream.swap32ABigEndian(len);
	uint32_t id= ID_NOT_INTERESTED;
	id= this->bitStream.swap32ABigEndian(id);
	return int32Astring(len) + int32Astring(id);
}
			
std::string ProtocoloBitTorrent::have(std::string piece) {
	uint32_t len= 5;
	len= this->bitStream.swap32ABigEndian(len);
	uint32_t id= ID_HAVE;
	id = this->bitStream.swap32ABigEndian(id);
	return int32Astring(len) + int32Astring(id) + piece;
}

std::string ProtocoloBitTorrent::bitfield(std::string bitfield) {
	uint32_t len= 1 + bitfield.length();
	len= this->bitStream.swap32ABigEndian(len);
	uint32_t id = ID_BITFIELD;
	id= this->bitStream.swap32ABigEndian(id);
	return int32Astring(len) + int32Astring(id) + bitfield;
}
					
std::string ProtocoloBitTorrent::request(int index, int begin, int length) {
	//length = 2^14(16KB)
	uint32_t len= 13;
	len= this->bitStream.swap32ABigEndian(len);
	uint32_t id= ID_REQUEST;
	id= this->bitStream.swap32ABigEndian(id);
	index= this->bitStream.swap32ABigEndian(index);
	begin= this->bitStream.swap32ABigEndian(begin);
	length= this->bitStream.swap32ABigEndian(length);
	return int32Astring(len) + int32Astring(id) +
	       int32Astring(index) + int32Astring(begin) +
	       int32Astring(length);
}
			
std::string ProtocoloBitTorrent::piece(int index, int begin, std::string block) {
	uint32_t len= 9 + block.length();
	len= this->bitStream.swap32ABigEndian(len);
	uint32_t id= ID_PIECE;
	id= this->bitStream.swap32ABigEndian(id);
	index= this->bitStream.swap32ABigEndian(index);
	begin= this->bitStream.swap32ABigEndian(begin);
	return int32Astring(len) + int32Astring(id) + int32Astring(index) +
	       int32Astring(begin) + block;
}
			
std::string ProtocoloBitTorrent::cancel(int index, int begin, int length) {
	uint32_t len= 13; 
	len= this->bitStream.swap32ABigEndian(len);
	uint32_t id= ID_CANCEL;
	id = this->bitStream.swap32ABigEndian(id);
	index= this->bitStream.swap32ABigEndian(index);
	begin= this->bitStream.swap32ABigEndian(begin);
	length= this->bitStream.swap32ABigEndian(length);
	return int32Astring(len) + int32Astring(id) + int32Astring(index) +
	       int32Astring(begin) + int32Astring(length);
}
			
std::string ProtocoloBitTorrent::port(int listenPort) {
	uint32_t len= 3;
	len= this->bitStream.swap32ABigEndian(len);
	uint32_t id= ID_PORT;
	id= this->bitStream.swap32ABigEndian(id);
	listenPort= this->bitStream.swap32ABigEndian(listenPort);
	return  int32Astring(len) + int32Astring(id) + int32Astring(listenPort);
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
	
	std::string lenght;
	lenght.assign(msg, 0, 4);
	
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
			message->piece.assign(msg, 5, 4);

		} else if(id == ID_BITFIELD) {
			std::cout << "bitfield" << std::endl;
			message->id= BITFIELD;
			message->bitfield.assign(msg, 5, atoi(lenght.c_str())-1);

		} else if(id == ID_REQUEST) {
			std::cout << "request" << std::endl;
			message->id= REQUEST;
			auxiliar.assign(msg, 5, 1);
			message->index= atoi(auxiliar.c_str());
			auxiliar.assign(msg, 6, 1);
			message->begin= atoi(auxiliar.c_str());
			auxiliar.assign(msg, 7, atoi(lenght.c_str())-8);
			message->length= atoi(auxiliar.c_str());

		} else if(id == ID_PIECE) {
			std::cout << "piece" << std::endl;
			message->id= PIECE;
			auxiliar.assign(msg, 5, 1);
			message->index= atoi(auxiliar.c_str());
			auxiliar.assign(msg, 6, 1);
			message->begin= atoi(auxiliar.c_str());
			auxiliar.assign(msg, 7, atoi(lenght.c_str())-7);
			message->block= atoi(auxiliar.c_str());
	
		} else if(id == ID_CANCEL) {
			std::cout << "cancel" << std::endl;
			message->id= CANCEL;
			auxiliar.assign(msg, 5, 1);
			message->index= atoi(auxiliar.c_str());
			auxiliar.assign(msg, 6, 1);
			message->begin= atoi(auxiliar.c_str());
			auxiliar.assign(msg, 7, atoi(lenght.c_str())-8);
			message->length= atoi(auxiliar.c_str());
		
		} else if(id == ID_PORT) {
			std::cout << "port" << std::endl;
			message->id= PORT;
			auxiliar.assign(msg, 5, 2);
			message->listenPort= atoi(auxiliar.c_str());
			
		}
	} else 
		message->id= KEEP_ALIVE;
	
	return message;
}

