#include "shell.h"

/****************************************************************************/
Shell::Shell(Controlador* controlador) {
	
	this->controlador= controlador;
}
		
/*--------------------------------------------------------------------------*/
void mostrarHelp() {
	
	std::cout << std::endl;
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

void commandNoEncontrado() {
	std::cerr << "Comando no encontrado. ";
	std::cerr << "Intente con el comando help." << std::endl;
}

void Shell::correr() {
	
	system("clear");	
	std::cout << std::endl;
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
			
		else if(command.compare(CLEAR) == 0)
			system("clear");		
		
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
		
		std::cout << std::endl;
		
	}
}

/*--------------------------------------------------------------------------*/
void mostrarSubrayado(int max) {
	
	std::cout.width(max);
	std::cout.fill(TITLE);
	std::cout << std::left << TITLE << std::endl;
}

/*--------------------------------------------------------------------------*/
/**Archivo**/
void Shell::mostrarArchivos() {

	std::cout << std::endl;	 							  	
	std::cout << "++ FILES ++"<< std::endl;
	std::cout << "==========="<< std::endl;
	
	std::cout.width(MAX_AMOUNT);
	std::cout.fill(RELLENO);
	std::cout << std::left << "#";
	std::cout.width(MAX_FILE);
	std::cout.fill(RELLENO);
	std::cout << std::left << "File";
	std::cout.width(MAX_PIECE);
	std::cout.fill(RELLENO);
	std::cout << std::left << "Piece"; 
	std::cout.width(MAX_SIZE);
	std::cout.fill(RELLENO);
	std::cout << std::left << "Size";
	std::cout.width(MAX_DONE);
	std::cout.fill(RELLENO);
	std::cout << std::left << "Done";
	std::cout.width(MAX_STATUS);
	std::cout.fill(RELLENO);
	std::cout << std::left << "Status";
	std::cout.width(MAX_SPEED);
	std::cout.fill(RELLENO);
	std::cout << std::left << "Up Speed";
	std::cout.width(MAX_SPEED);
	std::cout.fill(RELLENO);
	std::cout << std::left << "Down Speed"; 
	std::cout << std::endl;
	mostrarSubrayado(MAX_FILES);
}	

void Shell::mostrarArchivo(int amount, std::string file, int piece, 
  						   std::string size, int done, std::string status,
  						   std::string upSpeed, std::string downSpeed) {
	
	if(file.length() > MAX_FILE)
		file.resize(MAX_FILE-1);
		
	std::cout.width(MAX_AMOUNT);
	std::cout.fill(RELLENO);
	std::cout << std::left << amount;
	std::cout.width(MAX_FILE);
	std::cout.fill(RELLENO);
	std::cout << std::left << file;
	std::cout.width(MAX_PIECE);
	std::cout.fill(RELLENO);
	std::cout << std::left << piece;
	std::cout.width(MAX_SIZE);
	std::cout.fill(RELLENO);
	std::cout << std::left << size; 
	std::cout.width(MAX_DONE);
	std::cout.fill(RELLENO);
	std::cout << std::left << done;	
	std::cout.width(MAX_STATUS);
	std::cout.fill(RELLENO);
	std::cout << std::left << status; 
	std::cout.width(MAX_SPEED);
	std::cout.fill(RELLENO);
	std::cout << std::left << upSpeed; 
	std::cout.width(MAX_SPEED);
	std::cout.fill(RELLENO);
	std::cout << std::left << downSpeed;
	std::cout << std::endl;
}

/*--------------------------------------------------------------------------*/
/**General**/
void Shell::mostrarGeneral(std::string file, std::string downloaded,
						   std::string information) {

	std::cout << std::endl;	 							  	
	std::cout << "++ GENERAL ++"<< std::endl;
	std::cout << "============="<< std::endl;
	
	std::cout << "File: " << file << std::endl;
	std::cout << "Downloaded: " << downloaded << std::endl;
	std::cout << "Information: " << information << std::endl;		
}

/*--------------------------------------------------------------------------*/
/**Trackers**/
void Shell::mostrarTrackers() {

	std::cout << std::endl;	 							  	
	std::cout << "++ TRACKERS ++"<< std::endl;
	std::cout << "=============="<< std::endl;
		
	std::cout.width(MAX_NAME);
	std::cout.fill(RELLENO);
	std::cout << std::left << "Nombre"; 
	std::cout.width(MAX_STATUS);
	std::cout.fill(RELLENO);
	std::cout << std::left << "Status";
	std::cout.width(MAX_SEED);
	std::cout.fill(RELLENO);
	std::cout << std::left << "Seed";  
	std::cout << std::endl;
	mostrarSubrayado(MAX_TRACKERS);
}

void Shell::mostrarTracker(std::string name, std::string status, int seed) {
	
	if(name.length() > MAX_NAME)
		name.resize(MAX_NAME-1);
	
	std::cout.width(MAX_NAME);
	std::cout.fill(RELLENO);
	std::cout << std::left << name;
	std::cout.width(MAX_STATUS);
	std::cout.fill(RELLENO);
	std::cout << std::left << status;
	std::cout.width(MAX_SEED);
	std::cout.fill(RELLENO);
	std::cout << std::left << seed;
	std::cout << std::endl;		
}

/*--------------------------------------------------------------------------*/
/*Peers*/
void Shell::mostrarPeers() {

	std::cout << std::endl;	 							  	
	std::cout << "++ PEERS ++"<< std::endl;
	std::cout << "==========="<< std::endl;
		
	std::cout.width(MAX_CLIENT);
	std::cout.fill(RELLENO);
	std::cout << std::left << "Cliente";
	std::cout.width(MAX_IP);
	std::cout.fill(RELLENO);
	std::cout << std::left << "IP";
	std::cout << std::endl;	
	mostrarSubrayado(MAX_PEERS);
}

void Shell::mostrarCliente(std::string ip, std::string cliente) {
	
	if(cliente.length() > MAX_CLIENT)
		cliente.resize(MAX_CLIENT-1);
	
	std::cout.width(MAX_CLIENT);
	std::cout.fill(RELLENO);
	std::cout << std::left << cliente;
	std::cout.width(MAX_IP);
	std::cout.fill(RELLENO);
	std::cout << std::left << ip;
	std::cout << std::endl;	
}

/*--------------------------------------------------------------------------*/
/**Pieces**/
void Shell::mostrarPieces() {

	std::cout << std::endl;	 							  	
	std::cout << "++ PIECES ++"<< std::endl;
	std::cout << "============"<< std::endl;
	
	std::cout.width(MAX_PIECE);
	std::cout.fill(RELLENO);
	std::cout << std::left << "#";
	std::cout.width(MAX_SIZE);
	std::cout.fill(RELLENO);
	std::cout << std::left << "Size";
	std::cout.width(MAX_BLOCKS);
	std::cout.fill(RELLENO);
	std::cout << std::left << "# of Blocks";
	std::cout.width(MAX_BLOCK);
	std::cout.fill(RELLENO);		 
	std::cout << std::left << "Block";
	std::cout.width(MAX_COMPLETED);
	std::cout.fill(RELLENO);
	std::cout << std::left << "Completed"; 
	std::cout << std::endl;		
	mostrarSubrayado(MAX_PIECES);
}

void Shell::mostrarPiece(int number, std::string size, int blocks, int block,
		                  int completed) {
	 
	std::cout.width(MAX_PIECE);
	std::cout.fill(RELLENO); 
	std::cout << std::left << number;
	std::cout.width(MAX_SIZE);
	std::cout.fill(RELLENO);
	std::cout << std::left << size;
	std::cout.width(MAX_BLOCKS);
	std::cout.fill(RELLENO);		
	std::cout << std::left << blocks;
	std::cout.width(MAX_BLOCK);
	std::cout.fill(RELLENO);		
	std::cout << std::left << block;
	std::cout.width(MAX_COMPLETED);
	std::cout.fill(RELLENO);
	std::cout << std::left << completed;
	std::cout << std::endl;		                   	
}

/*--------------------------------------------------------------------------*/
/**Mensajes**/
void Shell::mostrarMensaje(std::string mensaje) {
	
	std::cout << mensaje << std::endl;		 		
}

/****************************************************************************/
