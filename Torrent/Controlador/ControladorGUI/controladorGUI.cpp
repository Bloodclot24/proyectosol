#include "controladorGUI.h"

/****************************************************************************/
ControladorGUI::ControladorGUI(): mutexActividades(), mutexMensaje(), 
                                  mutexGeneral(), mutexPeers() {
	
	this->vista= new VistaTorrent(this);
	this->all= 0;
	this->downloading= 0;
	this->completed= 0;
	this->active= 0;
	//this->ventanaCargando= new VentanaCargando(this);
	//ventanaCargando->start();
	//cargarConfig();
}

/*--------------------------------------------------------------------------*/
ControladorGUI::~ControladorGUI() {

	//ventanaCargando->stop();	
	//guardarConfig();
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
	
	mutexActividades.lock();
	
	char sAux[15];
	sprintf(sAux, "%d", all);
	vista->modificarCantAll(sAux);
	sprintf(sAux, "%d", downloading);
	vista->modificarCantDownloading(sAux);
	sprintf(sAux, "%d", completed);
	vista->modificarCantCompleted(sAux);
	sprintf(sAux, "%d", active);
	vista->modificarCantActive(sAux);
	if(all-active >= 0)
		sprintf(sAux, "%d", all-active);
	else
		sprintf(sAux, "%d", all);		
	vista->modificarCantInactive(sAux);
	
	mutexActividades.unlock();	
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::mostrarAnnounceUrlTorrent(Torrent* torrent) {
	
	if(torrent->getAnnounceUrl().length() != 0)
		vista->agregarTracker(torrent->getAnnounceUrl(), "Disponible");
	else {
		const std::list<std::string>* listaUrl= torrent->getAnnounceUrlList();
		std::list<std::string>::const_iterator it;
		for(it = listaUrl->begin(); it != listaUrl->end(); it++)
			vista->agregarTracker((*it), "Disponible");
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
	std::string status= obtenerStatus(torrent->getState());
	std::string downSpeed= obtenerVelocidad(0);
	std::string upSpeed= obtenerVelocidad(0);
	std::string ETA= obtenerETA(0);
		
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
		if((vista->getEstadoFile(file).compare("Downloading")) == 0) {
			this->downloading--;
			this->active--;
		}
		actualizarCantActividades();
		if(all == 0) {
			limpiarPestanias();
			vista->limpiarListaTrackers();
			vista->limpiarMessages();
		}
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

		actualizarPestanias(file);
				
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

		actualizarPestanias(file);
		
	} else 
		vista->agregarMessage("ERROR: No se pudo PAUSAR la descarga de "
			                  + file);
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::stopFile(std::string file) {

	if(cliente->stop(file.c_str())) {
		
		if((vista->getEstadoFile(file).compare("Seeding")) == 0)	
			vista->complete(file);
		else {
			vista->stop(file);
			this->downloading--;			
		}
			
		this->active--;		
		actualizarCantActividades();

		actualizarPestanias(file);

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
void ControladorGUI::modificarGeneral(Torrent* torrent) {
		                              	
	mutexGeneral.lock();
	
	double done= torrent->getPorcentaje();
	std::string doneS= obtenerDownloaded(done);
	std::string status= obtenerStatus(torrent->getState());
	vista->modificarFilename(torrent->getName());
	vista->modificarDownloaded(doneS);
	vista->modificarInformacion(status);
	
	mutexGeneral.unlock();		                              	
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::modificarPeers(Torrent* torrent) {
		                              	
	mutexPeers.lock();
	
	vista->limpiarListaClientes();
	std::list<std::string> lista= torrent->getListaPeers();
	std::list<std::string>::iterator it;
	for(it= lista.begin(); it != lista.end(); it++) {
		vista->agregarCliente((*it), "");
	}
	
	mutexPeers.unlock();		                              	
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::actualizarPestanias(std::string filename) {
	
	Torrent* torrent= obtenerTorrent(filename);

	modificarGeneral(torrent);
	//modificarPeers(torrent);
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::limpiarPestanias() {
	
	//-General-
	mutexGeneral.lock();
	
	vista->modificarFilename("");
	vista->modificarDownloaded("");
	vista->modificarInformacion("");
	
	mutexGeneral.unlock();
	
	//-Peers-
	mutexPeers.lock();
		
	vista->limpiarListaClientes();
	
	mutexPeers.unlock();
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
	
	this->active++;	
	this->downloading++;
	actualizarCantActividades();
	
	vista->start(filename);
	
	if(vista->archivoSeleccionado(filename)) {
		Torrent* torrent= obtenerTorrent(filename);
		modificarGeneral(torrent);
	}
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::pause(std::string filename) {
	
	this->active--;		
	this->downloading--;
	actualizarCantActividades();
	
	vista->pause(filename);
	
	if(vista->archivoSeleccionado(filename)) {
		Torrent* torrent= obtenerTorrent(filename);
		modificarGeneral(torrent);
	}
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::stop(std::string filename) {
	
	this->active--;		
	this->downloading--;		
	actualizarCantActividades();
	
	vista->stop(filename);
	
	if(vista->archivoSeleccionado(filename)) {
		Torrent* torrent= obtenerTorrent(filename);
		modificarGeneral(torrent);
	}
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::complete(std::string filename) {

	this->downloading--;		
	this->completed++;		
	actualizarCantActividades();
	
	vista->complete(filename);	

//	if(vista->archivoSeleccionado(filename)) {
//		Torrent* torrent= obtenerTorrent(filename);
//		modificarGeneral(torrent);
//	}
}	

/*--------------------------------------------------------------------------*/
void ControladorGUI::seed(std::string filename) {

	this->downloading--;		
	this->completed++;		
	actualizarCantActividades();	

	vista->seed(filename);			
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::actualizarDone(std::string file, int done) {

	vista->actualizarDone(file, done);
	
//	if(vista->archivoSeleccionado(file)) {
//		Torrent* torrent= obtenerTorrent(file);
//		modificarGeneral(torrent);
//	}
	
	std::cout << "Filename:" << file << std::endl;
	std::cout << "Done:" << done << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::actualizarStatus(std::string file, std::string status) {
	
	vista->actualizarStatus(file, status);
	actualizarCantActividades();
	
//	if(vista->archivoSeleccionado(file)) {
//		Torrent* torrent= obtenerTorrent(file);
//		modificarGeneral(torrent);
//	}
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::actualizarDownSpeed(std::string file, 
		                                             uint32_t downSpeed) {
	
	vista->actualizarDownSpeed(file, obtenerVelocidad(downSpeed));
//	
//	std::cout << "Nombre archivo dp de actualizar down speed: " << file << std::endl;
//	
//	Torrent* torrent= obtenerTorrent(file);
//	double done= torrent->getPorcentaje();
//	double size = torrent->getTotalSize();
//	uint32_t time = 0;
//	if(downSpeed != 0){
//		time = (size *( 100 - done)/100) / downSpeed;
//		vista->actualizarTime(file,obtenerETA(time));
//	}
//	
//	std::cout << "Nombre archivo final: " << file << std::endl;
//	std::cout << "------------------------------------------" << std::endl;
//	
//	
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
/**Message**/
/*--------------------------------------------------------------------------*/
void ControladorGUI::agregarMessage(std::string message) {
	
	mutexMensaje.lock();
	
	//vista->agregarMessage(message);
	
	mutexMensaje.unlock();
}

/****************************************************************************/
