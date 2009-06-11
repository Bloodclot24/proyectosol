#include "shell.h"

/****************************************************************************/
Shell::Shell(Controlador* controlador) {
	
	this->controlador= controlador;
}
		
/*--------------------------------------------------------------------------*/
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

		if(command.compare(SHOW_TRACKERS) == 0)
			controlador->mostrarTrackers();
					
		else if(command.compare(SHOW_FILES) == 0)
			controlador->mostrarFiles();

		else if(command.compare(HELP) == 0)
			mostrarHelp();
		
		else if(finParametro != std::string::npos) {
			
			file.assign(command, finParametro+1, 
			                               command.length()-finParametro);
						
			if(command.compare(0, finParametro, REMOVE) == 0)
				controlador->removeFile(file);
			else if(command.compare(0, finParametro, START) == 0)
				controlador->startFile(file);
			else if(command.compare(0, finParametro, PAUSE) == 0)
				controlador->pauseFile(file);
			else if(command.compare(0, finParametro, STOP) == 0)
				controlador->stopFile(file);
			else if(command.compare(0, finParametro, SHOW_PIECES) == 0)
				controlador->mostrarPieces(file);
			else if(command.compare(0, finParametro, SHOW_PEERS) == 0)
				controlador->mostrarPeers(file);
			else if(command.compare(0, finParametro,SHOW_GENERAL) == 0)
				controlador->mostrarGeneral(file);	
			else if(command.compare(0, finParametro, ADD_TORRENT) == 0)
				controlador->addTorrent(file);
	//		else if(command.compare(0, finParametro, ADD_URL_TORRENT) == 0)
	//			controlador->addUrlTorrent(file);				
			else
				commandNoEncontrado();
							
		} else if(command.compare(EXIT) != 0)
			commandNoEncontrado();
	}
	controlador->exit();	
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
	
	std::cout << ADD_TORRENT << " filename: "; 
	std::cout << "agrega un torrent a partir de un archivo." << std::endl;
//	std::cout << ADD_URL_TORRENT; 
//	std::cout << ": agrega un torrent a partir de un URL." << std::endl;
	std::cout << SHOW_TRACKERS << ": "; 
	std::cout << "muestra todos los trackers." << std::endl;
	std::cout << SHOW_FILES << ": "; 
	std::cout << "muestra todos los archivos." << std::endl;
	std::cout << REMOVE << " file#: ";
	std::cout << "se borra el # archivo." << std::endl;
	std::cout << START << " file#: "; 
	std::cout << "se comienza a bajar el # archivo." << std::endl;
	std::cout << PAUSE << " file#: "; 
	std::cout << "se pausa la bajada del # archivo." << std::endl;
	std::cout << STOP << " file#: "; 
	std::cout << "se detiene la bajada del # archivo." << std::endl;
	std::cout << SHOW_GENERAL << " file#: ";
	std::cout << "muestra la informacion general del # archivo." << std::endl;
	std::cout << SHOW_PEERS << " file#: "; 
	std::cout << "muestra los peers del # archivo." << std::endl;		 
	std::cout << SHOW_PIECES << " file#: ";
	std::cout << "se muestra las piezas del # archivo." << std::endl;
	std::cout << EXIT << ": ";; 
	std::cout << "salir del programa." << std::endl;		 
}

/*--------------------------------------------------------------------------*/
/*Mensajes*/
void Shell::mostrarMensaje(std::string mensaje) {
	
	std::cout << mensaje << std::endl;		 		
}

/****************************************************************************/
