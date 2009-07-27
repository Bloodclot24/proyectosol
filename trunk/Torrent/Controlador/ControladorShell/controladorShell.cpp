#include "controladorShell.h"

/****************************************************************************/
ControladorShell::ControladorShell() {

	this->shell= new Shell(this);	
	this->contadorOrden= 0;
	cargarConfig();
	crearAlertaFallo();
}

/*--------------------------------------------------------------------------*/
ControladorShell::~ControladorShell() {

	guardarConfig();
	delete shell;
}

/*--------------------------------------------------------------------------*/
void ControladorShell::correr() {
	
	shell->correr();
}

/*--------------------------------------------------------------------------*/
void ControladorShell::mostrarAnnounceUrlTorrent(Torrent* torrent) {
	
	if(torrent->getAnnounceUrl().length() != 0)
		shell->mostrarTracker(torrent->getAnnounceUrl(), "Disponible");
	else {
		const std::list<std::string>* listaUrl= torrent->getAnnounceUrlList();
		std::list<std::string>::const_iterator it;
		for(it = listaUrl->begin(); it != listaUrl->end(); it++)
			shell->mostrarTracker((*it), "Disponible");
	}
}

/*--------------------------------------------------------------------------*/
/* VISTA -> MODELO 
 * =============== */
/*--------------------------------------------------------------------------*/
bool ControladorShell::addTorrent(std::string pathTorrent) {
	
	bool valido= validarExtensionFile(pathTorrent);
	std::string filename= FileManager::obtenerFilename(pathTorrent);
	bool existe= cliente->existeTorrent(filename.c_str());
	
	if(valido && !existe) {
		std::string pathCopia= crearCopiaTorrent(pathTorrent);
		
		if(pathCopia.length() == 0) {
			shell->mostrarMessage("Error El archivo no existe");
			return false;
		}
		
		if(cliente->addTorrent(pathCopia.c_str()))
			return true;
		 else {
			shell->mostrarMessage("Error al cargar el archivo");
			return false;
		}
	} 
	
	if(!valido)
		shell->mostrarMessage("Debe seleccionar un archivo .torrent");
	else
		shell->mostrarMessage("El archivo seleccionado ya existe");
	
	return false;
}

/*--------------------------------------------------------------------------*/
void ControladorShell::addUrlTorrent() {
	
	/*UNDER CONSTRUCTION*/
}

/*--------------------------------------------------------------------------*/
void ControladorShell::mostrarTrackers() {
	
	shell->mostrarTrackers();
	
	const std::list<Torrent*>* listaTorrents= this->cliente->getListaTorrents();
	std::list<Torrent*>::const_iterator it;
	int contador= 1;
	for(it = listaTorrents->begin(); it != listaTorrents->end(); it++, contador++)
		mostrarAnnounceUrlTorrent(*it);				
} 

/*--------------------------------------------------------------------------*/
void ControladorShell::mostrarFiles() {
	
	shell->mostrarArchivos();
	const std::list<Torrent*>* listaTorrents= this->cliente->getListaTorrents();
	std::list<Torrent*>::const_iterator it;
	int contador= 1;
	std::string estado;
	for(it = listaTorrents->begin(); it != listaTorrents->end(); it++, contador++) {
		Torrent* torrent= *it;
		
		std::string name= torrent->getName();		
		std::string sizeS= obtenerSize(torrent->getTotalSize());
		std::string status= obtenerStatus(torrent->getState());
		
		uint32_t downSpeed= 0;
		uint32_t upSpeed= 0;
		double size= torrent->getTotalSize();
		double done= torrent->getPorcentaje();
		uint32_t time= 0;
					
		if(status == "Downloading" || status == "Seeding") {
			
			downSpeed= torrent->getVelocidadBajada();
			upSpeed= torrent->getVelocidadSubida();
			
			if(downSpeed != 0)
				time= (size *( 100 - done)/100) / downSpeed;
			if(upSpeed != 0)
				time= (size *( 100 - done)/100) / upSpeed;
		}	
		
		std::string ETAS= obtenerETA(time);
		std::string downSpeedS= obtenerVelocidad(downSpeed);
		std::string upSpeedS= obtenerVelocidad(upSpeed);
			
		shell->mostrarArchivo(contador, name, sizeS, done, status, downSpeedS, 
		                      upSpeedS, ETAS);		
	}	
}

/*--------------------------------------------------------------------------*/
void ControladorShell::mostrarPeers(std::string numFile) {

	bool valido= validarNumFile(numFile);
	
	std::string filename= obtenerFilename(numFile);
	
	if(valido) {
		Torrent* torrent= obtenerTorrent(filename);
		shell->mostrarPeers();
		std::list<std::string> lista= torrent->getListaPeers();
		std::list<std::string>::iterator it;
		for(it= lista.begin(); it != lista.end(); it++)
			shell->mostrarCliente(*it, "");
	}
}

/*--------------------------------------------------------------------------*/
void ControladorShell::removeFile(std::string numFile) {
	
	bool valido= validarNumFile(numFile);
	
	if(valido) {
		if(!cliente->remove(obtenerFilename(numFile)))
			shell->mostrarMessage("No se pudo BORRAR el archivo.");
	} else  	
		shell->mostrarMessage("No es un numero de archivo valido.");
}

/*--------------------------------------------------------------------------*/
void ControladorShell::startFile(std::string numFile) {

	bool valido= validarNumFile(numFile);
	
	if(valido) {
		if(!cliente->start(obtenerFilename(numFile)))
			shell->mostrarMessage("ERROR: No se pudo INICIAR la descarga.");
	} else
		shell->mostrarMessage("ERROR: No es un numero de archivo valido.");		
}

/*--------------------------------------------------------------------------*/
void ControladorShell::pauseFile(std::string numFile) {
	
	bool valido= validarNumFile(numFile);
	
	if(valido) {
		if(!cliente->pause(obtenerFilename(numFile)))
			shell->mostrarMessage("ERROR: No se pudo PAUSAR la descarga.");
	} else  	
		shell->mostrarMessage("ERROR: No es un numero de archivo valido.");
}

/*--------------------------------------------------------------------------*/
void ControladorShell::stopFile(std::string numFile) {
	
	bool valido= validarNumFile(numFile);
	
	if(valido) {
		if(!cliente->stop(obtenerFilename(numFile)))
			shell->mostrarMessage("ERROR: No se pudo DETENER la descarga.");
	} else  	
		shell->mostrarMessage("ERROR: No es un numero de archivo valido.");	
}

/*--------------------------------------------------------------------------*/
bool ControladorShell::validarNumFile(std::string numFile) {
	
	unsigned int num= atoi(numFile.c_str());
	
	if(num > 0 && num <= cliente->getListaTorrents()->size())
		return true;
	else
		return false;
}

/*--------------------------------------------------------------------------*/
uint32_t ControladorShell::obtenerOrden(std::string filename) {
	
	return(contadorOrden++);		
}

/*--------------------------------------------------------------------------*/
const char* ControladorShell::obtenerFilename(std::string numFile) {
	
	const std::list<Torrent*>* listaTorrents= this->cliente->getListaTorrents();
	std::list<Torrent*>::const_iterator it;
	int contador= 1;
	int buscado= atoi(numFile.c_str());
	bool encontrado= false;
	std::string filename;

	for(it= listaTorrents->begin(); it != listaTorrents->end() && !encontrado; 
	    it++, contador++) {

		if(contador ==  buscado) {
			encontrado= true;
			filename= (*it)->getName(); 
		}
	}	
	
	return filename.c_str();
}

/*--------------------------------------------------------------------------*/
/* MODELO -> VISTA 
 * =============== */		 
/*--------------------------------------------------------------------------*/
/*Message*/
/*--------------------------------------------------------------------------*/
void ControladorShell::agregarMessage(std::string message) {

	shell->mostrarMessage(message);
}

/****************************************************************************/
