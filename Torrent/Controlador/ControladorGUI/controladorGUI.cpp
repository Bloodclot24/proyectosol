#include "controladorGUI.h"

/****************************************************************************/
ControladorGUI::ControladorGUI(): mutexMensaje() {
	
	this->vista= new VistaTorrent(this);
	this->all= 0;
	this->downloading= 0;
	this->completed= 0;
	this->active= 0;
	this->ventanaCargando= new VentanaCargando(this);
	ventanaCargando->start();
	cargarConfig();
}

/*--------------------------------------------------------------------------*/
ControladorGUI::~ControladorGUI() {

	ventanaCargando->stop();	
	guardarConfig();
	delete vista;	
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::correr() {
	
	//estos botones no esta habilitados
	vista->disableAddUrlTorrent();
	vista->disablePause();
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
			vista->agregarTracker((*it), "Disponible", 
			                      torrent->getPeersActivos());
	}
}

/*--------------------------------------------------------------------------*/
/* VISTA -> MODELO 
 * =============== */
/*--------------------------------------------------------------------------*/
/**Files**/
/*--------------------------------------------------------------------------*/
void ControladorGUI::mostrarFile(Torrent* torrent) {

	std::string name= torrent->getName();
	std::string size= obtenerSize(torrent->getTotalSize());
	double done= torrent->getPorcentaje();
	std::string status= obtenerStatus(torrent->getEstado());
	std::string downSpeed= obtenerVelocidad(torrent->getVelocidadBajada());
	std::string upSpeed= obtenerVelocidad(torrent->getVelocidadSubida());
	
	/*HARDCODEADOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/
	std::string ETA= obtenerETA(3600*24+1);
		
	vista->agregarArchivo(name, size, done, status, downSpeed, upSpeed, ETA);	
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::mostrarFiles() {
	
  	for(uint32_t i= 0; i < listaOrdenada.size(); i++) {
		
		this->all++;
		Torrent* torrent= obtenerTorrent(listaOrdenada[i]);
		mostrarFile(torrent);
  	}		
		
	actualizarCantActividades();					  
}

/*--------------------------------------------------------------------------*/
bool ControladorGUI::addTorrent(std::string pathTorrent) {

	//checkeo que sea un .torrent
	bool valido= validarExtensionFile(pathTorrent);
	std::string filename= FileManager::obtenerFilename(pathTorrent);
	bool existe= cliente->existeTorrent(filename.c_str());
	
	if(valido && !existe) {
	     //ventanaCargando->show();
		
		std::string pathCopia= crearCopiaTorrent(pathTorrent);
			
		if(cliente->addTorrent(pathCopia.c_str())) {
			
		     //while(!vista->isVisibleDelayDialog());

			//vista->cerrarDelayDialog();	
			vista->cerrarFileChooser();

			this->all++;
			actualizarCantActividades();
			
			filename= FileManager::obtenerFilename(pathCopia);
			Torrent* torrent= obtenerTorrent(filename);
			mostrarFile(torrent);
			mostrarAnnounceUrlTorrent(torrent);
			vista->borrarMensaje();
			
			return true;
		} else {
			vista->cerrarDelayDialog();
			vista->mostrarMensaje("Error al cargar el archivo");
			return false;
		}
	}
	
	vista->cerrarDelayDialog();
	if(!valido)
		vista->mostrarMensaje("Debe seleccionar un archivo .torrent");
	else
		vista->mostrarMensaje("El archivo seleccionado ya existe");
	return false;
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::addUrlTorrent() {
	
	/*UNDER CONSTRUCTION*/
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::removeFile(std::string file) {
	
	if(cliente->remove(file.c_str())) { 
		this->all--;
		actualizarCantActividades();	
	} else 
		vista->agregarMessage("ERROR: No se pudo borrar el archivo" + file);
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::startFile(std::string file) {
	
	if(cliente->start(file.c_str())) {
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
	
	if(cliente->pause(file.c_str())) {
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

	if(cliente->stop(file.c_str())) {
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
	
	const std::list<Torrent*>* listaTorrents= this->cliente->getListaTorrents();
	std::list<Torrent*>::const_iterator it;
	for(it = listaTorrents->begin(); it != listaTorrents->end(); it++)
		mostrarAnnounceUrlTorrent(*it);			
}

/*--------------------------------------------------------------------------*/
/**Todas las Pestanias**/
void ControladorGUI::actualizarPestanias(std::string filename) {
	
	Torrent* torrent= obtenerTorrent(filename);

	//-General-
	double done= torrent->getPorcentaje();
	std::string doneS= obtenerDownloaded(done);
	std::string status= obtenerStatus(torrent->getEstado());
	vista->modificarFilename(filename);
	vista->modificarDownloaded(doneS);
	vista->modificarInformacion(status);
	
	//-Peers-
	vista->limpiarListaClientes();
	std::list<std::string> lista= torrent->getListaPeers();
	std::list<std::string>::iterator it;
	for(it= lista.begin(); it != lista.end(); it++) {
		vista->agregarCliente((*it), "");
	}
}

/*--------------------------------------------------------------------------*/
uint32_t ControladorGUI::obtenerOrden(std::string filename) {
	
	return(vista->obtenerOrden(filename));
}

/*--------------------------------------------------------------------------*/
/** DialogDelay**/
void ControladorGUI::mostrarDialogDelay() {
	
	vista->correrDelayDialog();
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
void ControladorGUI::actualizarDone(std::string file, int done) {

	vista->actualizarDone(file, done);
	std::string doneS;
	std::stringstream cvz;
	cvz.width();
	cvz << trunc(done);
	doneS= cvz.str();
	doneS += " %";
	vista->modificarDownloaded(doneS);
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::actualizarStatus(std::string file, std::string status) {
	
	vista->actualizarStatus(file, status);
	vista->modificarInformacion(status);
	actualizarCantActividades();		                              	
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::actualizarDownSpeed(std::string file, 
		                                             uint32_t downSpeed) {
	
	vista->actualizarDownSpeed(file, obtenerVelocidad(downSpeed));
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::actualizarUpSpeed(std::string file, 
                                       uint32_t upSpeed) {	
	
	vista->actualizarUpSpeed(file, obtenerVelocidad(upSpeed));
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::actualizarTime(std::string file, int time) {

	vista->actualizarTime(file, obtenerETA(time));	
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
	
	vista->modificarSeedTracker(name, seed);
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::eliminarTracker(std::string name) {
	
	vista->eliminarTracker(name);
}

/*--------------------------------------------------------------------------*/
/**Message**/
/*--------------------------------------------------------------------------*/
void ControladorGUI::agregarMessage(std::string message) {
	
	mutexMensaje.lock();
	std::cout << "MENSAJE VISTA: --------> " << message << std::endl;
	vista->agregarMessage(message);
	mutexMensaje.unlock();
}

/****************************************************************************/
