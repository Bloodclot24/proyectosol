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
void Shell::mostrarArchivos() {
	
	std::cout << "File" << ESPACIO << "#" << ESPACIO; 
	std::cout << "Size" << ESPACIO << "Done" << ESPACIO;
	std::cout << "Status" << ESPACIO << "Up Speed" << ESPACIO;
	std::cout << "Down Speed" << std::endl;
}	

void Shell::mostrarArchivo(std::string file, int piece, std::string size,
		                    int done, std::string status, std::string 
		                    downSpeed, std::string upSpeed) {
		
		                    	
}

/*--------------------------------------------------------------------------*/
/*General*/
void Shell::mostrarGeneralDownloaded(std::string downloaded,
		 							 std::string availability) {
		 							  	
	std::cout << "General"<< std::endl;
	std::cout << "-------"<< std::endl;
	std::cout << "Downloaded: " << downloaded << std::endl;
	std::cout << "Availability: " << availability << std::endl;	
}

/*--------------------------------------------------------------------------*/
/*Trackers*/
void Shell::mostrarTrackers() {
	
	std::cout << "TRACKERS"<< std::endl;
	std::cout << "--------"<< std::endl;
	std::cout << "Nombre" << ESPACIO << "Status";
	std::cout << ESPACIO << "Seed" << std::endl; 	
}

void Shell::mostrarTracker(std::string name, std::string status, int seed) {

	std::cout << name << ESPACIO;
	std::cout << status << ESPACIO;
	std::cout << seed << std::endl;		
}

/*--------------------------------------------------------------------------*/
/*Peers*/
void Shell::mostrarPeers() {
	
	std::cout << "PEERS"<< std::endl;
	std::cout << "-----"<< std::endl;
	std::cout << "Cliente" << ESPACIO << "IP" << std::endl;	
}

void Shell::mostrarCliente(std::string ip, std::string cliente) {
	
	std::cout << cliente << ESPACIO;
	std::cout << ip << std::endl;
}

/*--------------------------------------------------------------------------*/
/*Pieces*/
void Shell::mostrarPieces() {
	
	std::cout << "PIECES"<< std::endl;
	std::cout << "------"<< std::endl;
	std::cout << "#" << ESPACIO << "Size" << ESPACIO << "# of Blocks";		 
	std::cout << ESPACIO << "Block" << ESPACIO << "Completed" << std::endl;		
}

void Shell::mostrarPiece(int number, std::string size, int blocks, int block,
		                  int completed) {
	 
	std::cout << number << ESPACIO;
	std::cout << size << ESPACIO;
	std::cout << blocks << ESPACIO;
	std::cout << block << ESPACIO;
	std::cout << completed << std::endl;	                   	
}

/****************************************************************************/
