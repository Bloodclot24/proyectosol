#include "controladorShell.h"

/****************************************************************************/
ControladorShell::ControladorShell() {

	this->shell= new Shell(this);	
}

/*--------------------------------------------------------------------------*/
ControladorShell::~ControladorShell() {

	delete shell;
}

/*--------------------------------------------------------------------------*/
void ControladorShell::correr() {
	
	shell->correr();
}

/*--------------------------------------------------------------------------*/
bool ControladorShell::addTorrent(std::string pathTorrent) {
	
	//INFORMAR TORRENT NUEVO
	//FALTA TODO LO QUE SIGNIFICA AGREGAR UN NUEVO TORRENT
	bool valido= validarExtensionFile(pathTorrent);
	
	if(valido) {
		std::cout << "Path Valido: " << pathTorrent << std::endl;
//		if(cliente.addTorrent(pathTorrent.c_str()))
//			return true;
//		else {
//			shell->mostrarMensaje("Error al cargar el archivo");
//			return false;
//		}
	} else {
		shell->mostrarMensaje("Debe seleccionar un archivo .torrent");
		return false;
	}
}

/*--------------------------------------------------------------------------*/
void ControladorShell::addUrlTorrent() {
	
	/*UNDER CONSTRUCTION*/
}

/*--------------------------------------------------------------------------*/
void ControladorShell::mostrarTrackers() {
	
	shell->mostrarTrackers();
	//PEDIR TRACKERS
	shell->mostrarTracker("Vero", "Disponible", 5);
} 

/*--------------------------------------------------------------------------*/
void ControladorShell::mostrarFiles() {
	
	shell->mostrarArchivos();
	//PEDIR TODOS LOS ARCHIVOS
	shell->mostrarArchivo("Nothing else matters - Metalica", 4, "120 mb",
		                    45, "Downloading", "12.2 kb/s", "");
}

/*--------------------------------------------------------------------------*/
void ControladorShell::mostrarGeneral(std::string numFile) {

	//PEDIR DATOS
	bool valido= validarNumFile(numFile);
	
	if(valido) {
		std::cout << "Valido" << std::endl;	
		shell->mostrarGeneral( numFile, "80 kb/s", "Waiting...");
	}
}

/*--------------------------------------------------------------------------*/
void ControladorShell::mostrarPeers(std::string numFile) {

	std::cout << "numFile: " << numFile << std::endl;	
	bool valido= validarNumFile(numFile);
	
	if(valido) {
		std::cout << "Valido" << std::endl;	
		shell->mostrarPeers();
		//PEDIR CLIENTES
		shell->mostrarCliente("127.0.0.1", "home");
	}
}

/*--------------------------------------------------------------------------*/
void ControladorShell::mostrarPieces(std::string numFile) {
	
	std::cout << "numFile: " << numFile << std::endl;	
	bool valido= validarNumFile(numFile);
	
	if(valido) {
		std::cout << "Valido" << std::endl;	
		shell->mostrarPieces();
		//PEDIR PIEZAS
		shell->mostrarPiece(1, "200 mb", 50, 4, 2);
	}
}

/*--------------------------------------------------------------------------*/
void ControladorShell::removeFile(std::string numFile) {
	
	std::cout << "numFile: " << numFile << std::endl;
	bool valido= validarNumFile(numFile);
	
	if(valido) {
		std::cout << "Valido" << std::endl;	
	}		
}

/*--------------------------------------------------------------------------*/
void ControladorShell::startFile(std::string numFile) {

	std::cout << "numFile: " << numFile << std::endl;
	bool valido= validarNumFile(numFile);
	
	if(valido) {
		std::cout << "Valido" << std::endl;	
	}		
}

/*--------------------------------------------------------------------------*/
void ControladorShell::pauseFile(std::string numFile) {
	
	std::cout << "numFile: " << numFile << std::endl;
	bool valido= validarNumFile(numFile);
	
	if(valido) {
		std::cout << "Valido" << std::endl;	
	}		
}

/*--------------------------------------------------------------------------*/
void ControladorShell::stopFile(std::string numFile) {
	
	std::cout << "numFile: " << numFile << std::endl;
	bool valido= validarNumFile(numFile);
	
	if(valido) {
		std::cout << "Valido" << std::endl;	
	}		
}

/*--------------------------------------------------------------------------*/
bool ControladorShell::validarNumFile(std::string numFile) {
	
	int num= atoi(numFile.c_str());
	
	if(num != 0 && num != INT_MAX && num != INT_MIN)
		return true;
	else
		return false;
}

/*--------------------------------------------------------------------------*/
void ControladorShell::exit() {

	std::cout << "EXIT" << std::endl;		
}

/****************************************************************************/