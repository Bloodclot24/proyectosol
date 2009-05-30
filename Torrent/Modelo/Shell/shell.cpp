#include "shell.h"

/****************************************************************************/
void Shell::correr() {

	this->command= "";
	
	while(strcmp(command.c_str(), EXIT) != 0) {
	
		std::cout << PROMPT;
		std::cin >> command;

		if(strcmp(command.c_str(), SHOW_TRACKERS) == 0) {
			
			std::cout << "SHOW_TRACKERS" << std::endl;
			
		} else if(strcmp(command.c_str(), SHOW_PEERS) == 0) {
			
			std::cout << "SHOW_PEERS" << std::endl;
			
		} else if(strcmp(command.c_str(), SHOW_TRACKERS) == 0) {
			
			std::cout << "SHOW_GENERAL" << std::endl;
			
		} else if(strcmp(command.c_str(), SHOW_GENERAL) == 0) {
			
			std::cout << "SHOW_ALL" << std::endl;
			
		} else if(strcmp(command.c_str(), SHOW_ALL) == 0) {
			
			std::cout << "SHOW_DOWNLOADING" << std::endl;
			
		} else if(strcmp(command.c_str(), SHOW_DOWNLOADING) == 0) {
			
			std::cout << "SHOW_COMPLETED" << std::endl;
			
		} else if(strcmp(command.c_str(), SHOW_ACTIVE) == 0) {
			
			std::cout << "SHOW_ACTIVE" << std::endl;
			
		} else if(strcmp(command.c_str(), SHOW_INACTIVE) == 0) {
			
			std::cout << "SHOW_INACTIVE" << std::endl;
			
		} else if(strcmp(command.c_str(), ADD_TORRENT) == 0) {
			
			std::cout << "ADD_TORRENT" << std::endl;
			
		} else if(strcmp(command.c_str(), ADD_URL_TORRENT) == 0) {
 
			std::cout << "ADD_URL_TORRENT" << std::endl;			
		} 
	}
	
}

/*--------------------------------------------------------------------------*/
/*Archivo*/
void Shell::mostrarArchivo(std::string file, int piece, std::string size,
		                    int done, std::string status, std::string 
		                    downSpeed, std::string upSpeed) {
		
		                    	
}

/*--------------------------------------------------------------------------*/
/*General*/
void Shell::mostrarGeneralDownloaded(std::string downloaded,
		 							  std::string availability) {
		 							  	

}

/*--------------------------------------------------------------------------*/
/*Trackers*/
void Shell::mostrarTracker(std::string name, std::string status, int seed) {


}

/*--------------------------------------------------------------------------*/
/*Peers*/
void Shell::mostrarCliente(std::string ip, std::string cliente) {
	
	
}

/*--------------------------------------------------------------------------*/
/*Pieces*/
void Shell::mostrarPieces(int number, std::string size, int blocks, int block,
		                  int completed) {
		 
		                   	
}

/****************************************************************************/
