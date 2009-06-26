#include "controladorShell.h"

/****************************************************************************/
ControladorShell::ControladorShell() {

	this->shell= new Shell(this);	
	this->cantFiles= 1;
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
bool ControladorShell::addTorrent(std::string pathTorrent) {
	
	bool valido= validarExtensionFile(pathTorrent);
	
	if(valido) {
		std::string filename=  crearCopiaTorrent(pathTorrent);

		if(cliente.addTorrent(filename.c_str())) {
			this->cantFiles++;
			return true;
		} else {
			shell->mostrarMensaje("Error al cargar el archivo");
			return false;
		}
	} else {
		shell->mostrarMensaje("Debe seleccionar un archivo .torrent");
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
	//PEDIR TRACKERS
	//shell->mostrarTracker("Vero", "Disponible", 5);
} 

/*--------------------------------------------------------------------------*/
void ControladorShell::mostrarFiles() {
	
	shell->mostrarArchivos();
	std::list<Torrent*>* listaTorrents= this->cliente.getListaTorrents();
	std::list<Torrent*>::iterator it;
	int contador= 1;
	std::string estado;
	for(it = listaTorrents->begin(); it != listaTorrents->end(); it++, contador++) {
		Torrent* torrent= *it;
		estado= getEstadoTorrent(torrent->getEstadoTorrent());
		shell->mostrarArchivo(contador, torrent->getName(), 0, "", 0, estado, "", "");		
	}	
}

/*--------------------------------------------------------------------------*/
void ControladorShell::mostrarGeneral(std::string numFile) {

	bool valido= validarNumFile(numFile);
	
	if(valido) {
		Torrent* torrent= obtenerTorrent(obtenerFilename(numFile));
		std::string estado= getEstadoTorrent(torrent->getEstadoTorrent());
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
			shell->mostrarMensaje("No se pudo BORRAR el archivo.");
	} else  	
		shell->mostrarMensaje("No es un numero de archivo valido.");
}

/*--------------------------------------------------------------------------*/
void ControladorShell::startFile(std::string numFile) {

	bool valido= validarNumFile(numFile);
	
	if(valido) {
		if(!cliente.start(obtenerFilename(numFile)))
			shell->mostrarMensaje("ERROR: No se pudo INICIAR la descarga.");
	} else
		shell->mostrarMensaje("ERROR: No es un numero de archivo valido.");		
}

/*--------------------------------------------------------------------------*/
void ControladorShell::pauseFile(std::string numFile) {
	
	bool valido= validarNumFile(numFile);
	
	if(valido) {
		if(!cliente.pause(obtenerFilename(numFile)))
			shell->mostrarMensaje("ERROR: No se pudo PAUSAR la descarga.");
	} else  	
		shell->mostrarMensaje("ERROR: No es un numero de archivo valido.");
}

/*--------------------------------------------------------------------------*/
void ControladorShell::stopFile(std::string numFile) {
	
	bool valido= validarNumFile(numFile);
	
	if(valido) {
		if(!cliente.stop(obtenerFilename(numFile)))
			shell->mostrarMensaje("ERROR: No se pudo DETENER la descarga.");
	} else  	
		shell->mostrarMensaje("ERROR: No es un numero de archivo valido.");	
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
	
	std::list<Torrent*>* listaTorrents= this->cliente.getListaTorrents();
	std::list<Torrent*>::iterator it;
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
void agregarMessage(std::string message) {

	shell->mostrarMessage(message);
}

/****************************************************************************/
