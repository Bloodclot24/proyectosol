#include "protocoloBitTorrent.h"

std::string ProtocoloBitTorrent::handshake(std::string str, std::string info_hash, 
                                           std::string peer_id) {
     char pstrlen= str.length();

     std::string mensajeHandshake;
     std::string reserved("\0\0\0\0\0\0\0\0",8);
     mensajeHandshake= pstrlen + str + reserved + info_hash + peer_id;
     return mensajeHandshake;
}  
			
std::string ProtocoloBitTorrent::keepAlive() {
     uint32_t len= 0;
     len= htonl(len);
     return int32Astring(len);
}
			
std::string ProtocoloBitTorrent::choke() {
     uint32_t len= 1;
     len= htonl(len);
     char id = ID_CHOKE;
     std::string aux((char*)&len, 4);           
     aux += id;                                 
     return aux;
}
			
std::string ProtocoloBitTorrent::unchoke() {
     uint32_t len= 1;
     len= htonl(len);
     char id= ID_UNCHOKE;
     std::string aux((char*)&len, 4);           
     aux += id;                                 
     return aux;
}
			
std::string ProtocoloBitTorrent::interested() {
     uint32_t len= 1;
     len= htonl(len);
     char id= ID_INTERESTED;
     std::string aux((char*)&len, 4);           
     aux += id;                                 
     return aux;
}
			
std::string ProtocoloBitTorrent::not_interested() {
     uint32_t len= 1;
     len= htonl(len);
     char id = ID_NOT_INTERESTED;
     std::string aux((char*)&len, 4);           
     aux += id;                                 
     return aux;
}
			
std::string ProtocoloBitTorrent::have(uint32_t piece) {
     uint32_t len= 5;
     len= htonl(len);
     char id= ID_HAVE;
     std::string aux((char*)&len, 4);           
     aux += id;
     len= htonl(piece);
     std::string aux1((char*)&len, 4);           
     aux += aux1;                                 
     return aux;
}

std::string ProtocoloBitTorrent::bitfield(uint32_t length) {
     uint32_t len= 1 + length;
     len= htonl(len);
     char id = ID_BITFIELD;
     std::string aux((char*)&len, 4);           
     aux += id;
     return aux;
}
					
std::string ProtocoloBitTorrent::request(uint32_t index, uint32_t begin, uint32_t length) {
     uint32_t len= 13;
     len= htonl(len);
     char id = ID_REQUEST;
     index= htonl(index);
     begin= htonl(begin);
     length= htonl(length);
     std::string aux((char*)&len, 4);           
     aux += id;
     std::string aux1((char*)&index,4);
     aux += aux1;
     std::string aux2((char*)&begin,4);
     aux += aux2;
     std::string aux3((char*)&length,4);
     aux += aux3;                              
     return aux;
}
			
std::string ProtocoloBitTorrent::piece(uint32_t index, uint32_t begin, uint32_t length){
     uint32_t len= 9 + length;
     len= htonl(len);
     char id= ID_PIECE;
     index= htonl(index);
     begin= htonl(begin);
     std::string aux((char*)&len, 4);
     aux += id;
     aux += std::string((char*)&index,4);
     aux += std::string((char*)&begin,4);
     return aux;
}
			
std::string ProtocoloBitTorrent::cancel(uint32_t index, uint32_t begin, uint32_t length) {
     uint32_t len= 13; 
     len= htonl(len);
     char id= ID_CANCEL;
     index= htonl(index);
     begin= htonl(begin);
     length= htonl(length);
     std::string aux((char*)&len, 4);           
     aux += id;
     std::string aux1((char*)&index,4);
     aux += aux1;
     std::string aux2((char*)&begin,4);
     aux += aux2;
     std::string aux3((char*)&length,4);
     aux += aux3;                              
     return aux;
}
			
std::string ProtocoloBitTorrent::port(uint32_t listenPort) {
     uint32_t len= 3;
     len= htonl(len);
     char id= ID_PORT;
     listenPort= htonl(listenPort);
     std::string aux((char*)&len, 4);           
     aux += id;
     std::string aux1((char*)&listenPort,4);
     aux += aux1;
     return aux;
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
Message* ProtocoloBitTorrent::decode(Deque<char> &deque) {
     uint32_t bytes = 0;
     char aux[4];
     //Obtengo los primeros 4 bytes
     while(bytes<4){
	  aux[bytes] = deque.popFront();
	  bytes++;
     }
			
     Message* message= new Message();
     memset(message, 0, sizeof(Message));
	
     uint32_t* longitudMsj = (uint32_t*)aux;
     uint32_t longMsj = ntohl(*longitudMsj);

//     std::cout << "LONGITUD===============" << longMsj << "\n"; 

     if(longMsj != 0) {
	  //Obtengo el id, para lo cual, leo el proximo byte
	  bytes = 0;
	  aux[bytes] = deque.popFront();
	  int id= aux[0];
	  //  std::cout << "ID=================" << id << "\n";
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
	       message->id= HAVE;
	       bytes = 0;
	       while(bytes < (4)){
		    aux[bytes] = deque.popFront();
		    bytes++;
	       }
	       message->index = ntohl(*(uint32_t*)aux);

	  } else if(id == ID_BITFIELD) {
	       message->id= BITFIELD;
	       message->length = longMsj-1;

	  } else if(id == ID_REQUEST) {
	       std::cout << "request" << std::endl;
	       message->id= REQUEST;
	       bytes = 0;
	       while(bytes < 4){
		    aux[bytes] = deque.popFront();
		    bytes++;
	       }
	       message->index = ntohl(*(uint32_t*)aux);
	       bytes = 0;
	       while(bytes < 4){
		    aux[bytes] = deque.popFront();
		    bytes++;
	       }
	       message->begin = ntohl(*(uint32_t*)aux);

	       bytes = 0;
	       while(bytes < 4){
		    aux[bytes] = deque.popFront();
		    bytes++;
	       }
	       message->length = ntohl(*(uint32_t*)aux);

	  } else if(id == ID_PIECE) {
	       std::cout << "piece" << std::endl;
	       message->id= PIECE;
	       bytes = 0;
	       while(bytes < 4){
		    aux[bytes] = deque.popFront();
		    bytes++;
	       }
	       message->index = ntohl(*(uint32_t*)aux);

	       bytes = 0;
	       while(bytes < 4){
		    aux[bytes] = deque.popFront();
		    bytes++;
	       }
	       message->begin = ntohl(*(uint32_t*)aux);
	       message->length = longMsj-9;
	
	  } else if(id == ID_CANCEL) {
	       std::cout << "cancel" << std::endl;
	       message->id = CANCEL;
	       bytes = 0;
	       while(bytes < 4){
		    aux[bytes] = deque.popFront();
		    bytes++;
	       }
	       message->index = ntohl(*(uint32_t*)aux);
			
	       bytes = 0;
	       while(bytes < 4){
		    aux[bytes] = deque.popFront();
		    bytes++;
	       }

	       message->begin = ntohl(*(uint32_t*)aux);

	       bytes = 0;
	       while(bytes < 4){
		    aux[bytes] = deque.popFront();
		    bytes++;
	       }

	       message->length = ntohl(*(uint32_t*)aux);

	  } else if(id == ID_PORT) {
	       std::cout << "port" << std::endl;
	       message->id= PORT;
	       bytes = 0;
	       while(bytes < 4){
		    aux[bytes] = deque.popFront();
		    bytes++;
	       }
	       message->listenPort = ntohl(*(uint32_t*)aux);
	  }
     } else {
	  message->id= KEEP_ALIVE;
	  std::cout << "keepAlive\n";
     }
     
	
     return message;
}

