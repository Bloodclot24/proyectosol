#include "shell.h"
#include "../../Controlador/controlador.h"

/****************************************************************************/
void commandNoEncontrado() {
	std::cerr << "Comando no encontrado. ";
	std::cerr << "Intente con el comando help." << std::endl;
}

void Shell::correr() {

	this->command= "";
	std::string file= "";
	
	while(command.compare(EXIT) != 0) {
	
		std::cout << PROMPT;
		std::getline(std::cin, command);
		size_t finParametro= command.find(" ", 0);

		if(command.compare(SHOW_TRACKERS) == 0) {
			Controlador::getInstancia()->mostrarTrackers();
			
		} else if(command.compare(SHOW_PEERS) == 0) {
			Controlador::getInstancia()->mostrarPeers();

		} else if(command.compare(SHOW_GENERAL) == 0) {
			Controlador::getInstancia()->mostrarGeneral();	
			
		} else if(command.compare(SHOW_ALL) == 0) {
			Controlador::getInstancia()->mostrarAll();
			
		} else if(command.compare(SHOW_DOWNLOADING) == 0) {
			Controlador::getInstancia()->mostrarDownloading();
			
		} else if(command.compare(SHOW_COMPLETED) == 0) {
			Controlador::getInstancia()->mostrarCompleted();
			
		} else if(command.compare(SHOW_ACTIVE) == 0) {
			Controlador::getInstancia()->mostrarActive();
			
		} else if(command.compare(SHOW_INACTIVE) == 0) {
			Controlador::getInstancia()->mostrarInactive();
			
		} else if(command.compare(ADD_TORRENT) == 0) {
			Controlador::getInstancia()->mostrarAddTorrent();
			
		} else if(command.compare(ADD_URL_TORRENT) == 0) {
			Controlador::getInstancia()->mostrarAddUrlTorrent();
		
		} else if(command.compare(HELP) == 0) {
			mostrarHelp();
		
		} else if(finParametro != std::string::npos) {
			
			file.assign(command, finParametro+1, 
			                               command.length()-finParametro);
						
			if(command.compare(0, finParametro, REMOVE) == 0)
				Controlador::getInstancia()->removeFile(file);
			else if(command.compare(0, finParametro, START) == 0)
				Controlador::getInstancia()->startFile(file);
			else if(command.compare(0, finParametro, PAUSE) == 0)
				Controlador::getInstancia()->pauseFile(file);
			else if(command.compare(0, finParametro, STOP) == 0)
				Controlador::getInstancia()->stopFile(file);
			else if(command.compare(0, finParametro, SHOW_PIECES) == 0)
				Controlador::getInstancia()->mostrarPieces(file);
			else
				commandNoEncontrado();
							
		} else if(command.compare(EXIT) != 0)
			commandNoEncontrado();
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
	
	std::cout << ADD_TORRENT; 
	std::cout << ": agrega un torrent a partir de un archivo." << std::endl;
	std::cout << ADD_URL_TORRENT; 
	std::cout << ": agrega un torrent a partir de un URL." << std::endl;
	std::cout << SHOW_TRACKERS; 
	std::cout << ": muestra todos los trackers." << std::endl;
	std::cout << SHOW_PEERS; 
	std::cout << " #: muestra los peers del # tracker." << std::endl;		 
	std::cout << SHOW_GENERAL; 
	std::cout << ": muestra la informacion general." << std::endl;
	std::cout << SHOW_ALL; 
	std::cout << ": muestra todos los archivos." << std::endl;
	std::cout << SHOW_DOWNLOADING; 
	std::cout << ": muestra todos los archivos que ";  
	std::cout << "se estan bajando." << std::endl;		 
	std::cout << SHOW_COMPLETED; 
	std::cout << ": muestra todos los archivos ";  
	std::cout << "completos." << std::endl;		 
	std::cout << SHOW_ACTIVE; 
	std::cout << ": muestra todos los archivos activos." << std::endl;		 
	std::cout << SHOW_INACTIVE; 
	std::cout << ": muestra todos los archivos inactivos." << std::endl;
	std::cout << REMOVE; 
	std::cout << " #: se borra el # archivo." << std::endl;
	std::cout << START; 
	std::cout << " #: se comienza a bajar el # archivo." << std::endl;
	std::cout << PAUSE; 
	std::cout << " #: se pausa la bajada del # archivo." << std::endl;
	std::cout << STOP; 
	std::cout << " #: se detiene la bajada del # archivo." << std::endl;
	std::cout << SHOW_PIECES; 
	std::cout << " #: se muestra las piezas del # archivo." << std::endl;
	std::cout << EXIT; 
	std::cout << ": salir del programa." << std::endl;		 
}

/*--------------------------------------------------------------------------*/
/*Mensajes*/
void Shell::archivoNoEncontrado() {
	
	std::cerr << "El archivo seleccionado no existe" << std::endl;		 		
}

/****************************************************************************/
