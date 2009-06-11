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
void ControladorShell::mostrarGeneral() {
	
	//PEDIR DATOS
	shell->mostrarGeneralDownloaded( "120 kb/s", "80 kb/s");
}

/*--------------------------------------------------------------------------*/
void ControladorShell::mostrarTrackers() {
	
	shell->mostrarTrackers();
	//PEDIR TRACKERS
	shell->mostrarTracker("Vero", "Disponible", 5);
} 

/*--------------------------------------------------------------------------*/
void ControladorShell::mostrarPeers() {
	
	shell->mostrarPeers();
	//PEDIR CLIENTES
	shell->mostrarCliente("127.0.0.1", "home");
}

/*--------------------------------------------------------------------------*/
void ControladorShell::mostrarPieces(std::string numFile) {
	
	std::cout << "numFile: " << numFile << std::endl;	
	shell->mostrarPieces();
	//PEDIR PIEZAS
	shell->mostrarPiece(1, "200 mb", 50, 4, 2);
}

/*--------------------------------------------------------------------------*/
void ControladorShell::mostrarAll() {
	
	shell->mostrarArchivos();
	//PEDIR TODOS LOS ARCHIVOS
	shell->mostrarArchivo("Nothing else matters - Metalica", 4, "120 mb",
		                    45, "Downloading", "12.2 kb/s", "");
}

/*--------------------------------------------------------------------------*/
void ControladorShell::mostrarDownloading() {

	shell->mostrarArchivos();
	//PEDIR TODOS LOS ARCHIVOS BAJANDO
	shell->mostrarArchivo("Nothing else matters - Metalica", 4, "120 mb",
		                    45, "Downloading", "12.2 kb/s", "");	
}

/*--------------------------------------------------------------------------*/
void ControladorShell::mostrarCompleted() {
	
	shell->mostrarArchivos();
	//PEDIR TODOS LOS ARCHIVOS COMPLETOS
	shell->mostrarArchivo("Nothing else matters - Metalica", 4, "120 mb",
		                    45, "Completed", "12.2 kb/s", "");
}

/*--------------------------------------------------------------------------*/
void ControladorShell::mostrarActive() {
	
	shell->mostrarArchivos();
	//PEDIR TODOS LOS ARCHIVOS ACTIVOS
	shell->mostrarArchivo("Nothing else matters - Metalica", 4, "120 mb",
		                    45, "Downloading", "12.2 kb/s", "");	
}

/*--------------------------------------------------------------------------*/
void ControladorShell::mostrarInactive() {

	shell->mostrarArchivos();
	//PEDIR TODOS LOS ARCHIVOS INACTIVOS
	shell->mostrarArchivo("Nothing else matters - Metalica", 4, "120 mb",
		                    45, "Stopped", "12.2 kb/s", "");	
}

/*--------------------------------------------------------------------------*/
void ControladorShell::mostrarAddTorrent() {
	
	//INFORMAR TORRENT NUEVO
	
	//FALTA TODO LO QUE SIGNIFICA AGREGAR UN NUEVO TORRENT
}

/*--------------------------------------------------------------------------*/
void ControladorShell::mostrarAddUrlTorrent() {
	
	/*EN UN FUTURO??*/
}

/*--------------------------------------------------------------------------*/
void ControladorShell::removeFile(std::string numFile) {

	std::cout << "numFile: " << numFile << std::endl;		
}

/*--------------------------------------------------------------------------*/
void ControladorShell::startFile(std::string numFile) {

	std::cout << "numFile: " << numFile << std::endl;		
}

/*--------------------------------------------------------------------------*/
void ControladorShell::pauseFile(std::string numFile) {
	
	std::cout << "numFile: " << numFile << std::endl;		
}

/*--------------------------------------------------------------------------*/
void ControladorShell::stopFile(std::string numFile) {
	
	std::cout << "numFile: " << numFile << std::endl;		
}

/****************************************************************************/