#include "controladorGUI.h"

/****************************************************************************/
ControladorGUI::ControladorGUI() {
	
	this->vista= new VistaTorrent(this);
	this->refrescador= new Refrescador(this);
	this->all= 0;
	this->completed= 0;
	this->downloading= 0;
	this->active= 0;
	cargarConfig();
	crearAlertaFallo();
}

/*--------------------------------------------------------------------------*/
ControladorGUI::~ControladorGUI() {

	guardarConfig();
	refrescador->terminar();
	delete refrescador;
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
	if(all-active >= 0)	
		sprintf(sAux, "%d", all-active);
	else
		sprintf(sAux, "%d", 0);		
	vista->modificarCantInactive(sAux);
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
		std::string pathCopia= crearCopiaTorrent(pathTorrent);
			
		if(cliente->addTorrent(pathCopia.c_str())) {
			
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
			vista->mostrarMensaje("Error al cargar el archivo");
			return false;
		}
	}
	
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
		
		std::string status= vista->getEstadoFile(file); 
		
		if((status.compare("Completed")) == 0)
			this->completed--;							
		else
			this->downloading--;			
			
		this->active--;		
		actualizarCantActividades();
		actualizarPestanias(file);

		vista->stop(file);

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
		                              	
	double done= torrent->getPorcentaje();
	std::string doneS= obtenerDownloaded(done);
	std::string status= obtenerStatus(torrent->getState());
	vista->modificarFilename(torrent->getName());
	vista->modificarDownloaded(doneS);
	vista->modificarInformacion(status);
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::modificarPeers(Torrent* torrent) {
		                              	
	vista->limpiarListaClientes();
	std::list<std::string> lista= torrent->getListaPeers();
	std::list<std::string>::iterator it;
	if(lista.size() > 0) {
		for(it= lista.begin(); it != lista.end(); it++)
			vista->agregarCliente((*it), "");
	}	
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::actualizarPestanias(std::string filename) {
	
	Torrent* torrent= obtenerTorrent(filename);

	modificarGeneral(torrent);
	modificarPeers(torrent);
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::limpiarPestanias() {
	
	//-General-
	vista->modificarFilename("");
	vista->modificarDownloaded("");
	vista->modificarInformacion("");
	
	//-Peers-
	vista->limpiarListaClientes();
}

/*--------------------------------------------------------------------------*/
uint32_t ControladorGUI::obtenerOrden(std::string filename) {
	
	return(vista->obtenerOrden(filename));
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
	
	vista->agregarMessage(message);
}

/*--------------------------------------------------------------------------*/
void ControladorGUI::actualizarPantalla() {
	
	this->all= 0;
	this->completed= 0;
	this->downloading= 0;
	this->active= 0;
	
	const std::list<Torrent*>* listaTorrents= this->cliente->getListaTorrents();
	std::list<Torrent*>::const_iterator it;
	
	if(listaTorrents->size() > 0) {
		for(it= listaTorrents->begin(); it != listaTorrents->end(); it++) {
			
			std::string file= (*it)->getName();	
			double done= (*it)->getPorcentaje();
			std::string status= obtenerStatus((*it)->getState());
			uint32_t downSpeed= (*it)->getVelocidadBajada();
			uint32_t upSpeed= (*it)->getVelocidadSubida();
			double size= (*it)->getTotalSize();
			uint32_t time= 8639999;
	
			if(downSpeed != 0)
				time= (size *( 100 - done)/100) / downSpeed;

			vista->actualizarDone(file, done);
			vista->actualizarStatus(file, status);
			vista->actualizarUpSpeed(file, obtenerVelocidad(upSpeed));
			vista->actualizarDownSpeed(file, obtenerVelocidad(downSpeed));
			vista->actualizarTime(file,obtenerETA(time));
			
			this->all++;
			
			if(done == 100) {
				this->completed++;
				vista->actualizarDownSpeed(file, obtenerVelocidad(0));
			} if(status.compare("Downloading") == 0) {
				this->downloading++;
				this->active++;
			} else if(status.compare("Seeding") == 0 || status.compare("Waiting...") == 0) {
				this->active++;			
			}
		}
		
// 		if(listaTorrents->size() == 1) {
// 			Torrent* torrent= *(listaTorrents->begin());
// 			std::string doneS= obtenerDownloaded(torrent->getPorcentaje());
// 			vista->modificarDownloaded(doneS);
// 			std::string statusS= obtenerStatus(torrent->getState());
// 			vista->modificarInformacion(statusS);
// 			//modificarPeers(torrent);
// 		}	
	}

	actualizarCantActividades();
	vista->actualizarTransferencias();
}	

/*--------------------------------------------------------------------------*/
void ControladorGUI::comenzarARefrescarPantalla() {
	
	refrescador->comenzar();
}		

/****************************************************************************/
