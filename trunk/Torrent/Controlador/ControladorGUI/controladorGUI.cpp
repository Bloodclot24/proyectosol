#include "controladorGUI.h"

/****************************************************************************/
ControladorGUI::ControladorGUI() {
	
	this->vista= new VistaTorrent(this);
	this->all= 0;
	this->downloading= 0;
	this->completed= 0;
	this->active= 0;
}

/*--------------------------------------------------------------------------*/
ControladorGUI::~ControladorGUI() {

	std::cout << "ACA TOY destructorGUI\n";

	delete vista;	
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::correr() {
	
	//provisoriamente estos botones no estaran habilitados
	vista->disableAddUrlTorrent();
	vista->disableMoveUp();
	vista->disableMoveDown();
	vista->correr();
}

/*--------------------------------------------------------------------------*/
bool ControladorGUI::addTorrent(std::string pathTorrent) {

	//GESTIONAR CON CLIENTE
	//checkeo que sea un .torrent
	bool valido= validarExtensionFile(pathTorrent);
	
	if(valido) {
		std::string pathCopia= crearCopiaTorrent(pathTorrent);
			
		if(cliente.addTorrent(pathCopia.c_str())) {
			vista->cerrarFileChooser();
			this->all++;
			actualizarCantActividades();
			vista->agregarTracker("SUN", "working", 10);
			vista->agregarArchivo(pathTorrent, 1, "200 MB", 0, "Downloading", 
							  "", "");
			vista->borrarMensaje();
			
			return true;
		} else {
			vista->mostrarMensaje("Error al cargar el archivo");
			return false;
		}
	} else {
		vista->mostrarMensaje("Debe seleccionar un archivo .torrent");
		return false;
	}
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::addUrlTorrent() {
	
	/*UNDER CONSTRUCTION*/
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::removeFile(std::string file) {
	
	//GESTIONAR CON CLIENTE
	
	//ver si hay q restar downloading completed active
	this->all--;
	
	std::cout << "all: " << all << std::endl;
	std::cout << "downloading: " << downloading << std::endl;
	std::cout << "completed: " << completed << std::endl;
	std::cout << "active: " << active << std::endl;
	
	actualizarCantActividades();
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::startFile(std::string file) {
	
	//GESTIONAR CON CLIENTE
	//checkear si se trata de una bajada o subida y actualizar la velocidad!!
	
	//hay q restar en algun lado
	this->active++;	
	this->downloading++;
	actualizarCantActividades();
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::pauseFile(std::string file) {
	
	//GESTIONAR CON CLIENT
	this->active--;		
	this->downloading--;
	actualizarCantActividades();	
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::stopFile(std::string file) {

	//GESTIONAR CON CLIENT
	this->active--;		
	this->downloading--;		
	actualizarCantActividades();
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::exit() {

	//GESTIONAR CON CLIENT
	//tenemos que ver que se va a cerrar
	std::cout << "EXIT" << std::endl;
		
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::actualizarDone(std::string file, int piece, int done) {

	vista->actualizarDone(file, piece, done);	
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::actualizarStatus(std::string file, int piece, 
		                                std::string status) {
	
	vista->actualizarStatus(file, piece, status);
	actualizarCantActividades();		                              	
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::actualizarDownSpeed(std::string file, int piece, 
		                                   std::string downSpeed) {
	
	vista->actualizarDownSpeed(file, piece, downSpeed);
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::actualizarUpSpeed(std::string file, int piece, 
		                                 std::string upSpeed) {	
	
	vista->actualizarUpSpeed(file, piece, upSpeed);
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::actualizarCantActividades() {
	
	//PEDIR AL CLIENTE LAS CANTIDADES
	char sAux[15];
	sprintf(sAux, "%d", all);
	vista->modificarCantAll(sAux);
	sprintf(sAux, "%d", downloading);
	vista->modificarCantDownloading(sAux);
	sprintf(sAux, "%d", completed);
	vista->modificarCantCompleted(sAux);
	sprintf(sAux, "%d", active);
	vista->modificarCantActive(sAux);
	sprintf(sAux, "%d", all-active);
	vista->modificarCantInactive(sAux);	
}

/*--------------------------------------------------------------------------*/
/**Trackers**/
void ControladorGUI::modificarStatusTracker(std::string name, 
                                              std::string status) {
	
	vista->modificarStatusTracker(name, status);
}		

/*--------------------------------------------------------------------------*/
void ControladorGUI::agregarSeedTracker(std::string name, int seed) {
	
	vista->agregarSeedTracker(name, seed);
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::eliminarTracker(std::string name) {
	
	vista->eliminarTracker(name);
}

/*--------------------------------------------------------------------------*/
/**Todas las Pestanias**/
void ControladorGUI::actualizarPestanias(std::string filename) {
	
	//General
	vista->modificarFilename(filename);
	//pedir completado
	vista->modificarDownloaded("4%");
	//pedir estado del archivo 
	vista->modificarInformacion("Waiting...");
	
	//Peers
	vista->limpiarListaClientes();
	//pedir lista clientes y usar
	vista->agregarCliente("127.0.0.4", "ReverPass");
	
	//Pieces
	vista->limpiarListaPieces();
	//pedir lista clientes y usar
	vista->agregarPiece(13, "12 MB", 15, 4, 4);	
}

/*--------------------------------------------------------------------------*/
uint32_t ControladorGUI::obtenerOrden(std::string filename) {
	
	return(vista->obtenerOrden(filename));
}

/****************************************************************************/
