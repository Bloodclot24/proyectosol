#include "shell.h"
#include "../Controlador/controlador.h"

/****************************************************************************/
void Shell::correr() {

	this->command= "";
	
	while(command.compare(EXIT) != 0) {
	
		std::cout << PROMPT;
		std::cin >> command;

		if(command.compare(SHOW_TRACKERS) == 0) {
			
			std::cout << "SHOW_TRACKERS" << std::endl;
			
		} else if(command.compare(SHOW_PEERS) == 0) {
			
			std::cout << "SHOW_PEERS" << std::endl;
			
		} else if(command.compare(SHOW_GENERAL) == 0) {
			
			std::cout << "SHOW_GENERAL" << std::endl;
			
		} else if(command.compare(SHOW_ALL) == 0) {
			
			std::cout << "SHOW_ALL" << std::endl;
			
		} else if(command.compare(SHOW_DOWNLOADING) == 0) {
			
			std::cout << "SHOW_DOWNLOADING" << std::endl;
			
		} else if(command.compare(SHOW_COMPLETED) == 0) {
			
			std::cout << "SHOW_COMPLETED" << std::endl;
			
		} else if(command.compare(SHOW_ACTIVE) == 0) {
			
			std::cout << "SHOW_ACTIVE" << std::endl;
			
		} else if(command.compare(SHOW_INACTIVE) == 0) {
			
			std::cout << "SHOW_INACTIVE" << std::endl;
			
		} else if(command.compare(ADD_TORRENT) == 0) {
			
			std::cout << "ADD_TORRENT" << std::endl;
			
		} else if(command.compare(ADD_URL_TORRENT) == 0) {
 
			std::cout << "ADD_URL_TORRENT" << std::endl;			
		
		} else if(command.compare(SHOW_PIECES) == 0) {
			
			std::cout << "SHOW_PIECES" << std::endl;
		}
		 else if(command.compare(HELP) == 0) {
			mostrarHelp();			
		} else {
			std::cout << "Comando no encontrado. ";
			std::cout << "Intente con el comando help." << std::endl;
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
		                   upSpeed, std::string downSpeed) {

	std::cout << file << ESPACIO << piece << ESPACIO; 
	std::cout << size << ESPACIO << done << ESPACIO;
	std::cout << status << ESPACIO <<  upSpeed << ESPACIO;
	std::cout << downSpeed << std::endl;
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

/*--------------------------------------------------------------------------*/
void Shell::mostrarHelp() {
	
	std::cout << "t: agrega un torrent a partir de un archivo." << std::endl;
	std::cout << "ut: agrega un torrent a partir de un URL." << std::endl;
	std::cout << "trackers: muestra todos los trackers." << std::endl;
	std::cout << "peers #: muestra los peers del # tracker." << std::endl;		 
	std::cout << "general: muestra la informacion general." << std::endl;
	std::cout << "all: muestra todos los archivos." << std::endl;
	std::cout << "downloading: muestra todos los archivos que ";  
	std::cout << "se estan bajando." << std::endl;		 
	std::cout << "completed: muestra todos los archivos ";  
	std::cout << "completos." << std::endl;		 
	std::cout << "active: muestra todos los archivos ";  
	std::cout << "activos." << std::endl;		 
	std::cout << "inactive: muestra todos los archivos ";  
	std::cout << "inactivos." << std::endl;
	std::cout << "remove #: se borra el # archivo." << std::endl;
	std::cout << "start #: se comienza a bajar el # archivo." << std::endl;
	std::cout << "pause #: se pausa la bajada del # archivo." << std::endl;
	std::cout << "stop #: se detiene la bajada del # archivo." << std::endl;
	std::cout << "pieces #: se muestra las piezas del # archivo." << std::endl;
	std::cout << "exit: salir del programa." << std::endl;		 
}

/****************************************************************************/
