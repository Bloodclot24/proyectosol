#include "protocoloBitTorrent.h"


int main(int argc,char** argv)
{
	ProtocoloBitTorrent protocoloTorrent;
	std::string str = "Bit Torrent";
	std::string info_hash = "2fd4e1c6 7a2d28fc ed849ee1 bb76e739 1b93eb12";
	std::string peer_id ="-AZ2060-";
	
	std::string mensajeHS = protocoloTorrent.handshake(str,info_hash,peer_id);
	
	std::cout<<"El mensaje de handshake obtenido es: "<<mensajeHS<<std::endl;
	
	return 0;
}
