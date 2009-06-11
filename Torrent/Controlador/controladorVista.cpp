#include "controladorVista.h"

/****************************************************************************/
ControladorVista::ControladorVista() {
	
	this->vista= new VistaTorrent(this);
}

/*--------------------------------------------------------------------------*/
ControladorVista::~ControladorVista() {

	delete vista;	
}

/*--------------------------------------------------------------------------*/
void ControladorVista::correr() {
	
	//provisoriamente estos botones no estaran habilitados
	vista->disableAddUrlTorrent();
	vista->disableMoveUp();
	vista->disableMoveDown();
	vista->correr();
}

/*--------------------------------------------------------------------------*/
bool ControladorVista::addTorrent(std::string pathTorrent) {

	//GESTIONAR CON CLIENTE
	//checkeo que sea un .torrent
	bool valido= validarExtensionFile(pathTorrent);
	
	if(valido) {
		vista->cerrarFileChooser();
		actualizarCantActividades();
		vista->agregarTracker("SUN", "working", 10);
		vista->agregarArchivo(pathTorrent, 1, "200 MB", 0, "Downloading", 
							  "", "");
		//actualizarPestanias(pathTorrent);
		vista->borrarMensaje();
		return true;
	} else {
		vista->mostrarMensaje("Debe seleccionar un archivo .torrent");
		return false;
	}
}

/*--------------------------------------------------------------------------*/
void ControladorVista::addUrlTorrent() {
	
	/*UNDER CONSTRUCTION*/
}

/*--------------------------------------------------------------------------*/
void ControladorVista::removeFile(std::string file) {
	
	//GESTIONAR CON CLIENTE
}

/*--------------------------------------------------------------------------*/
void ControladorVista::startFile(std::string file) {
	
	//GESTIONAR CON CLIENTE
	//checkear si se trata de una bajada o subida y actualizar la velocidad!!
}

/*--------------------------------------------------------------------------*/
void ControladorVista::pauseFile(std::string file) {
	
	//GESTIONAR CON CLIENT
}

/*--------------------------------------------------------------------------*/
void ControladorVista::stopFile(std::string file) {

	//GESTIONAR CON CLIENT	
}

/*--------------------------------------------------------------------------*/
void ControladorVista::exit() {

	//GESTIONAR CON CLIENT
	//tenemos que ver que se va a cerrar
	std::cout << "EXIT" << std::endl;	
}

/*--------------------------------------------------------------------------*/
void ControladorVista::actualizarDone(std::string file, int piece, int done) {

	vista->actualizarDone(file, piece, done);	
}

/*--------------------------------------------------------------------------*/
void ControladorVista::actualizarStatus(std::string file, int piece, 
		                                std::string status) {
	
	vista->actualizarStatus(file, piece, status);
	actualizarCantActividades();		                              	
}

/*--------------------------------------------------------------------------*/
void ControladorVista::actualizarDownSpeed(std::string file, int piece, 
		                                   std::string downSpeed) {
	
	vista->actualizarDownSpeed(file, piece, downSpeed);
}

/*--------------------------------------------------------------------------*/
void ControladorVista::actualizarUpSpeed(std::string file, int piece, 
		                                 std::string upSpeed) {	
	
	vista->actualizarUpSpeed(file, piece, upSpeed);
}

/*--------------------------------------------------------------------------*/
void ControladorVista::actualizarCantActividades() {
	
	//PEDIR AL CLIENTE LAS CANTIDADES
	vista->modificarCantAll("1");
	vista->modificarCantDownloading("1");
//	vista->modificarCantCompleted(cantCompleted);
//	vista->modificarCantActive(cantActive);
//	vista->modificarCantInactive(cantInactive);	
}

/*--------------------------------------------------------------------------*/
/**Trackers**/
void ControladorVista::modificarStatusTracker(std::string name, 
                                              std::string status) {
	
	vista->modificarStatusTracker(name, status);
}		

/*--------------------------------------------------------------------------*/
void ControladorVista::agregarSeedTracker(std::string name, int seed) {
	
	vista->agregarSeedTracker(name, seed);
}

/*--------------------------------------------------------------------------*/
void ControladorVista::eliminarTracker(std::string name) {
	
	vista->eliminarTracker(name);
}

/*--------------------------------------------------------------------------*/
/**Todas las Pestanias**/
void ControladorVista::actualizarPestanias(std::string filename) {
	
	//General
	vista->modificarFilename(filename);
	//pedir completado y usar
	vista->modificarDownloaded("4%");
	
	//Peers
	vista->limpiarListaClientes();
	//pedir lista clientes y usar
	vista->agregarCliente("127.0.0.4", "ReverPass");
	
	//Pieces
	vista->limpiarListaPieces();
	//pedir lista clientes y usar
	vista->agregarPiece(13, "12 MB", 15, 4, 4);	
}

/****************************************************************************/
