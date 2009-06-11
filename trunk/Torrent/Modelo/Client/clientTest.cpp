#include "client.h"

int main(int argc,char**argv){

     if(argc < 2){
	  std::cout << "Debe especificar un archivo torrent." << std::endl;
	  return -1;
     }
     Client cliente;
 
     if(cliente.addTorrent(argv[1]) == 0){
	  std::cout << "Error, no se pudo agregar el torrent." << std::endl;
     }

     cliente.start();
}
