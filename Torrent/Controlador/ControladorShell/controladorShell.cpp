#include "controladorShell.h"

/****************************************************************************/
ControladorShell::ControladorShell() {

	this->shell= new Shell(this);	
	this->cantFiles= 0;
	this->contadorOrden= 0;
	cargarConfig();
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
		shell->mostrarTracker(torrent->getAnnounceUrl(), "Disponible",
			                          torrent->getPeersActivos());
	else {
		const std::list<std::string>* listaUrl= torrent->getAnnounceUrlList();
		std::list<std::string>::const_iterator it;
		for(it = listaUrl->begin(); it != listaUrl->end(); it++)
			shell->mostrarTracker((*it), "Disponible", torrent->getPeersActivos());
	}
}

/*--------------------------------------------------------------------------*/
/* VISTA -> MODELO 
 * =============== */
/*--------------------------------------------------------------------------*/
bool ControladorShell::addTorrent(std::string pathTorrent) {
	
	bool valido= validarExtensionFile(pathTorrent);
	
	if(valido) {
		std::string pathCopia= crearCopiaTorrent(pathTorrent);
		
		if(pathCopia.length() == 0) {
			shell->mostrarMessage("Error El archivo no existe");
			return false;
		}
		
		if(cliente.addTorrent(pathCopia.c_str())) {
			this->cantFiles++;
			return true;
		} else {
			shell->mostrarMessage("Error al cargar el archivo");
			return false;
		}
	} else {
		shell->mostrarMessage("Debe seleccionar un archivo .torrent");
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
	
	const std::list<Torrent*>* listaTorrents= this->cliente.getListaTorrents();
	std::list<Torrent*>::const_iterator it;
	int contador= 1;
	for(it = listaTorrents->begin(); it != listaTorrents->end(); it++, contador++)
		mostrarAnnounceUrlTorrent(*it);				
} 

/*--------------------------------------------------------------------------*/
void ControladorShell::mostrarFiles() {
	
	shell->mostrarArchivos();
	const std::list<Torrent*>* listaTorrents= this->cliente.getListaTorrents();
	std::list<Torrent*>::const_iterator it;
	int contador= 1;
	std::string estado;
	for(it = listaTorrents->begin(); it != listaTorrents->end(); it++, contador++) {
		Torrent* torrent= *it;
		estado= getEstadoTorrent(torrent->getEstado());
		shell->mostrarArchivo(contador, torrent->getName(), 0, "", 0, estado, "", "");		
	}	
}

/*--------------------------------------------------------------------------*/
void ControladorShell::mostrarGeneral(std::string numFile) {

	bool valido= validarNumFile(numFile);
	
	if(valido) {
		Torrent* torrent= obtenerTorrent(obtenerFilename(numFile));
		std::string estado= getEstadoTorrent(torrent->getEstado());
		shell->mostrarGeneral(obtenerFilename(numFile), "", estado);
	}
}

/*--------------------------------------------------------------------------*/
void ControladorShell::mostrarPeers(std::string numFile) {

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
	
	bool valido= validarNumFile(numFile);
	
	if(valido) {
		if(!cliente.remove(obtenerFilename(numFile)))
			shell->mostrarMessage("No se pudo BORRAR el archivo.");
	} else  	
		shell->mostrarMessage("No es un numero de archivo valido.");
}

/*--------------------------------------------------------------------------*/
void ControladorShell::startFile(std::string numFile) {

	bool valido= validarNumFile(numFile);
	
	if(valido) {
		if(!cliente.start(obtenerFilename(numFile)))
			shell->mostrarMessage("ERROR: No se pudo INICIAR la descarga.");
	} else
		shell->mostrarMessage("ERROR: No es un numero de archivo valido.");		
}

/*--------------------------------------------------------------------------*/
void ControladorShell::pauseFile(std::string numFile) {
	
	bool valido= validarNumFile(numFile);
	
	if(valido) {
		if(!cliente.pause(obtenerFilename(numFile)))
			shell->mostrarMessage("ERROR: No se pudo PAUSAR la descarga.");
	} else  	
		shell->mostrarMessage("ERROR: No es un numero de archivo valido.");
}

/*--------------------------------------------------------------------------*/
void ControladorShell::stopFile(std::string numFile) {
	
	bool valido= validarNumFile(numFile);
	
	if(valido) {
		if(!cliente.stop(obtenerFilename(numFile)))
			shell->mostrarMessage("ERROR: No se pudo DETENER la descarga.");
	} else  	
		shell->mostrarMessage("ERROR: No es un numero de archivo valido.");	
}

/*--------------------------------------------------------------------------*/
bool ControladorShell::validarNumFile(std::string numFile) {
	
	int num= atoi(numFile.c_str());
	
	if(num > 0 && num <= cantFiles)
		return true;
	else
		return false;
}

/*--------------------------------------------------------------------------*/
uint32_t ControladorShell::obtenerOrden(std::string filename) {
	
	return(++contadorOrden);		
}

/*--------------------------------------------------------------------------*/
const char* ControladorShell::obtenerFilename(std::string numFile) {
	
	const std::list<Torrent*>* listaTorrents= this->cliente.getListaTorrents();
	std::list<Torrent*>::const_iterator it;
	int contador= 1;
	int buscado= atoi(numFile.c_str());
	bool encontrado= false;
	std::string filename;

	for(it= listaTorrents->begin(); it != listaTorrents->end() && !encontrado; it++, contador++) {

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
void ControladorShell::agregarMessage(std::string message) {

	shell->mostrarMessage(message);
}

/****************************************************************************/
