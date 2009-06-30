#include "protocoloBitTorrent.h"

/****************************************************************************/
/*PRUEBA UNITARIA BITTORRENT*/
/****************************************************************************/
int main(int argc,char** argv) {
	ProtocoloBitTorrent protocoloTorrent;
	std::string str = "Bit Torrent";
	std::string info_hash = "2fd4e1c6 7a2d28fc ed849ee1 bb76e739 1b93eb12";
	std::string peer_id ="-AZ2060-";
	
	std::string mensajeHS = protocoloTorrent.handshake(str,info_hash,peer_id);
	
	std::cout<<"El mensaje de handshake obtenido es: "<<mensajeHS<<std::endl;
	
	std::string keep_alive = protocoloTorrent.keepAlive();
	std::cout<<"Keep alive codificado: " << keep_alive<<std::endl;
	Message* msjDeco = new Message();
	msjDeco = protocoloTorrent.decode(keep_alive.c_str());
	std::cout << "El id del mensaje keep alive esperado es: "<<10<<std::endl;
	std::cout << "El id del mensaje keep alive decodificado es: "<<msjDeco->id<<std::endl;
	
	std::string choke = protocoloTorrent.choke();
	std::cout<<"El mensaje choke codificado: "<<choke<<std::endl;
	msjDeco = protocoloTorrent.decode(choke.c_str());
	std::cout << "El id del mensaje choke esperado es: "<<0<<std::endl;
	std::cout << "El id del mensaje choke decodificado es: "<<msjDeco->id<<std::endl;
	
	std::string unchoke = protocoloTorrent.unchoke();
	std::cout<<"El mensaje unchoke codificado: "<<unchoke<<std::endl;
	msjDeco = protocoloTorrent.decode(unchoke.c_str());
	std::cout << "El id del mensaje unchoke esperado es: "<<1<<std::endl;
	std::cout << "El id del mensaje unchoke decodificado es: "<<msjDeco->id<<std::endl;
	
	std::string interested = protocoloTorrent.interested();
	std::cout<<"El mensaje interested codificado: "<<interested<<std::endl;
	msjDeco = protocoloTorrent.decode(interested.c_str());
	std::cout << "El id del mensaje interested esperado es: "<<2<<std::endl;
	std::cout << "El id del mensaje interested decodificado es: "<<msjDeco->id<<std::endl;

	std::string not_interested = protocoloTorrent.not_interested();
	std::cout<<"El mensaje not_interested codificado: "<<not_interested<<std::endl;
	msjDeco = protocoloTorrent.decode(not_interested.c_str());
	std::cout << "El id del mensaje interested esperado es: "<<3<<std::endl;
	std::cout << "El id del mensaje not_interested decodificado es: "<<msjDeco->id<<std::endl;

	return 0;
}

/****************************************************************************/
