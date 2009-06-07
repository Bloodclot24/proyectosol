#include "controlador.h"
Controlador Controlador::controlador;

/****************************************************************************/
Controlador::Controlador(): shell() {
    
}

/*--------------------------------------------------------------------------*/
Controlador* Controlador::getInstancia() {
	
	return(&controlador);
}

/*--------------------------------------------------------------------------*/
void Controlador::mostrarGeneral() {
	
	//PEDIR DATOS
	shell->mostrarGeneralDownloaded( "120 kb/s", "80 kb/s");
}

/*--------------------------------------------------------------------------*/
void Controlador::mostrarTrackers() {
	
	shell->mostrarTrackers();
	//PEDIR TRACKERS
	shell->mostrarTracker("Vero", "Disponible", 5);
} 

/*--------------------------------------------------------------------------*/
void Controlador::mostrarPeers() {
	
	shell->mostrarPeers();
	//PEDIR CLIENTES
	shell->mostrarCliente("127.0.0.1", "home");
}

/*--------------------------------------------------------------------------*/
void Controlador::mostrarPieces() {
	
	shell->mostrarPieces();
	//PEDIR PIEZAS
	shell->mostrarPiece(1, "200 mb", 50, 4, 2);
}

/*--------------------------------------------------------------------------*/
void Controlador::mostrarAll() {
	
	shell->mostrarArchivos();
	//PEDIR TODOS LOS ARCHIVOS
	shell->mostrarArchivo("Nothing else matters - Metalica", 4, "120 mb",
		                    45, "Downloading", "12.2 kb/s", "");
}

/*--------------------------------------------------------------------------*/
void Controlador::mostrarDownloading() {

	shell->mostrarArchivos();
	//PEDIR TODOS LOS ARCHIVOS BAJANDO
	shell->mostrarArchivo("Nothing else matters - Metalica", 4, "120 mb",
		                    45, "Downloading", "12.2 kb/s", "");	
}

/*--------------------------------------------------------------------------*/
void Controlador::mostrarCompleted() {
	
	shell->mostrarArchivos();
	//PEDIR TODOS LOS ARCHIVOS COMPLETOS
	shell->mostrarArchivo("Nothing else matters - Metalica", 4, "120 mb",
		                    45, "Completed", "12.2 kb/s", "");
}

/*--------------------------------------------------------------------------*/
void Controlador::mostrarActive() {
	
	shell->mostrarArchivos();
	//PEDIR TODOS LOS ARCHIVOS ACTIVOS
	shell->mostrarArchivo("Nothing else matters - Metalica", 4, "120 mb",
		                    45, "Downloading", "12.2 kb/s", "");	
}

/*--------------------------------------------------------------------------*/
void Controlador::mostrarInactive() {

	shell->mostrarArchivos();
	//PEDIR TODOS LOS ARCHIVOS INACTIVOS
	shell->mostrarArchivo("Nothing else matters - Metalica", 4, "120 mb",
		                    45, "Stopped", "12.2 kb/s", "");	
}

/*--------------------------------------------------------------------------*/
void Controlador::mostrarAddTorrent() {
	
	//INFORMAR TORRENT NUEVO
	
	//FALTA TODO LO QUE SIGNIFICA AGREGAR UN NUEVO TORRENT
}

/*--------------------------------------------------------------------------*/
void Controlador::mostrarAddUrlTorrent() {
	
	/*EN UN FUTURO??*/
}

/****************************************************************************/
