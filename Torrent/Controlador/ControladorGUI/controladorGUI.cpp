#include "controladorGUI.h"

/****************************************************************************/
ControladorGUI::ControladorGUI() {
	
	this->vista= new VistaTorrent(this);
	this->all= 0;
	this->downloading= 0;
	this->completed= 0;
	this->active= 0;
	this->refrescador= new RefreshVista(this);
	this->refrescador->start();
	cargarConfig();
}

/*--------------------------------------------------------------------------*/
ControladorGUI::~ControladorGUI() {

	this->refrescador->stop();	
	guardarConfig();
	delete vista;	
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::correr() {
	
	//provisoriamente estos botones no estaran habilitados
	vista->disableAddUrlTorrent();
	vista->correr();
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::actualizarCantActividades() {
	
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
void ControladorGUI::mostrarAnnounceUrlTorrent(Torrent* torrent) {
	
	if(torrent->getAnnounceUrl().length() != 0)
		vista->agregarTracker(torrent->getAnnounceUrl(), "Disponible",
			                          torrent->getPeersActivos());
	else {
		const std::list<std::string>* listaUrl= torrent->getAnnounceUrlList();
		std::list<std::string>::const_iterator it;
		for(it = listaUrl->begin(); it != listaUrl->end(); it++)
			vista->agregarTracker((*it), "Disponible", torrent->getPeersActivos());
	}
}

/*--------------------------------------------------------------------------*/
/* VISTA -> MODELO 
 * =============== */
/*--------------------------------------------------------------------------*/
/**Files**/
/*--------------------------------------------------------------------------*/
void ControladorGUI::mostrarFiles() {
	
	std::cout << "---MOSTRANDO LISTA---" << std::endl;

  	for(uint32_t i= 0; i < listaOrdenada.size(); i++) {
		this->all++;
		Torrent* torrent= obtenerTorrent(listaOrdenada[i]);
		std::string estado= getEstadoTorrent(torrent->getEstado());
		std::cout << "torrent: " << torrent->getName() << std::endl;
		vista->agregarArchivo(torrent->getName(), "", 0, estado, "", "", "");
  	}		
		
	actualizarCantActividades();					  
}

/*--------------------------------------------------------------------------*/
bool ControladorGUI::addTorrent(std::string pathTorrent) {

	//checkeo que sea un .torrent
	bool valido= validarExtensionFile(pathTorrent);
	
	if(valido) {
		refrescador->refrescar();
		
		std::string pathCopia= crearCopiaTorrent(pathTorrent);
			
		if(cliente.addTorrent(pathCopia.c_str())) {
			if(refrescador->hide())
				vista->cerrarDelayDialog();
				
			vista->cerrarFileChooser();
			this->all++;
			actualizarCantActividades();
			std::string filename= FileManager::obtenerFilename(pathCopia);
			Torrent* torrent= obtenerTorrent(filename);
			std::string estado= getEstadoTorrent(torrent->getEstado());
			vista->agregarArchivo(filename, "", 0, estado, "", "", "");			
			mostrarAnnounceUrlTorrent(torrent);
			vista->borrarMensaje();
			
			return true;
		} else {
			vista->cerrarDelayDialog();
			vista->mostrarMensaje("Error al cargar el archivo");
			return false;
		}
	} else {
		vista->cerrarDelayDialog();
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
	
	if(cliente.remove(file.c_str())) { 
		this->all--;
		actualizarCantActividades();	
	} else 
		vista->agregarMessage("ERROR: No se pudo borrar el archivo" + file);
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::startFile(std::string file) {
	
	if(cliente.start(file.c_str())) {
		vista->start(file);
		this->active++;	
		this->downloading++;
		actualizarCantActividades();
	} else 
		vista->agregarMessage("ERROR: No se pudo INICIAR la descarga de " 
		                      + file);
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::pauseFile(std::string file) {
	
	if(cliente.pause(file.c_str())) {
		vista->pause(file);
		this->active--;		
		this->downloading--;
		actualizarCantActividades();
	} else 
		vista->agregarMessage("ERROR: No se pudo PAUSAR la descarga de "
			                  + file);
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::stopFile(std::string file) {

	if(cliente.stop(file.c_str())) {
		vista->stop(file);
		this->active--;		
		this->downloading--;		
		actualizarCantActividades();
	} else 
		vista->agregarMessage("ERROR: No se pudo DETENER la descarga de "
			                  + file);
}

/*--------------------------------------------------------------------------*/
/**Trackers**/
/*--------------------------------------------------------------------------*/
void ControladorGUI::mostrarTrackers() {
	
	const std::list<Torrent*>* listaTorrents= this->cliente.getListaTorrents();
	std::list<Torrent*>::const_iterator it;
	for(it = listaTorrents->begin(); it != listaTorrents->end(); it++)
		mostrarAnnounceUrlTorrent(*it);			
}

/*--------------------------------------------------------------------------*/
/**Todas las Pestanias**/
void ControladorGUI::actualizarPestanias(std::string filename) {
	
	//General
	vista->modificarFilename(filename);
	//pedir completado
	vista->modificarDownloaded("4%");
	//pedir estado del archivo 
	//vista->modificarInformacion("Waiting...");
	
	//Peers
	vista->limpiarListaClientes();
	//pedir lista clientes y usar
	//vista->agregarCliente("127.0.0.4", "ReverPass");
	
	//Pieces
	vista->limpiarListaPieces();
	//pedir lista clientes y usar
	//vista->agregarPiece(13, "12 MB", 15, 4, 4);	
}

/*--------------------------------------------------------------------------*/
uint32_t ControladorGUI::obtenerOrden(std::string filename) {
	
	return(vista->obtenerOrden(filename));
}

/*--------------------------------------------------------------------------*/
/**Refrescador**/		
void ControladorGUI::refrescar() {
	
	refrescador->refrescar();
}

/*--------------------------------------------------------------------------*/
/** DialogDelay**/
void ControladorGUI::mostrarDialogDelay() {
	
	vista->correrDelayDialog();
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::esconderDialogDelay() {

	vista->cerrarDelayDialog();
}

/*--------------------------------------------------------------------------*/
/* MODELO -> VISTA 
 * =============== */		 
/*--------------------------------------------------------------------------*/
/**Files**/ 		
/*--------------------------------------------------------------------------*/
void ControladorGUI::start(std::string filename) {
	
	vista->start(filename);
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::pause(std::string filename) {
	
	vista->pause(filename);
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::stop(std::string filename) {
	
	vista->stop(filename);
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::actualizarDone(std::string file, int piece, int done) {

	vista->actualizarDone(file, done);	
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::actualizarStatus(std::string file, int piece, 
		                                               std::string status) {
	
	vista->actualizarStatus(file, status);
	actualizarCantActividades();		                              	
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::actualizarDownSpeed(std::string file, int piece, 
		                                             std::string downSpeed) {
	
	vista->actualizarDownSpeed(file, downSpeed);
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::actualizarUpSpeed(std::string file, int piece, 
		                                 std::string upSpeed) {	
	
	vista->actualizarUpSpeed(file, upSpeed);
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::actualizarTime(std::string file, std::string time) {

	vista->actualizarTime(file, time);	
}

/*--------------------------------------------------------------------------*/
/**Trackers**/
/*--------------------------------------------------------------------------*/
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
/**Message**/
/*--------------------------------------------------------------------------*/
void ControladorGUI::agregarMessage(std::string message) {
	
	vista->agregarMessage(message);
}

/****************************************************************************/
