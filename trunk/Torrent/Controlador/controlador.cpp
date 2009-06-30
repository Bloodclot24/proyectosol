#include "controlador.h"

/****************************************************************************/
Controlador::Controlador(): listaOrdenada() {

	FileManager::crearDirectorio(PATH_DOWNLOADS);
	FileManager::crearDirectorio(PATH_CONFIG);
	this->cliente= new Client(this);
}

/*--------------------------------------------------------------------------*/
Controlador::~Controlador() { };

/*--------------------------------------------------------------------------*/
bool Controlador::validarExtensionFile(std::string path) {
	
	size_t pos= path.find(EXTENSION, path.length()-TAM_EXTENSION);
	
	if(pos != std::string::npos)
		return true;
	else
		return false; 
}

/*--------------------------------------------------------------------------*/
std::string Controlador::crearCopiaTorrent(std::string pathTorrent) {
	
	std::fstream file;
  	file.open(pathTorrent.c_str(), std::fstream::in);
  	std::string ruta;
		
	if(file.is_open()) {
		file.seekg(0, std::fstream::end);
		uint32_t last= file.tellg();
		file.seekg(0, std::fstream::beg);
		char* buffer= new char[last*sizeof(char)];
		file.read(buffer,last);
		std::fstream fileCopia;
		ruta+= PATH_CONFIG;
		ruta+= FileManager::obtenerFilename(pathTorrent);
		fileCopia.open(ruta.c_str(), std::fstream::out);
	  	fileCopia.write(buffer, last); 
	  	file.close();
	  	fileCopia.close();
	}
	
  	return ruta;
}

/*--------------------------------------------------------------------------*/
bool Controlador::guardarConfig() {
	
	bool resultado= false;
	std::fstream bitfieldFile;
	std::fstream config;
	uint32_t contador = 0;
	std::string snumero;
	uint32_t auxiliar;	
	const std::list<Torrent*>* listaTorrents  = this->cliente->getListaTorrents();
	std::list<Torrent*>::const_iterator it;
	config.open(NAME_FILE_CONFIG , std::fstream::out);
	if(!bitfieldFile.is_open()) resultado = false;
		auxiliar= listaTorrents->size();
		config.write((const char*)&auxiliar,sizeof(uint32_t));	
		
	for(it = listaTorrents->begin(); it != listaTorrents->end(); it++,contador++){
		std::stringstream cvz;
		BitField* bitfield = (*it)->getBitField();
   		cvz <<  contador;
   		snumero = cvz.str();
   		std::string ruta;
		ruta += PATH_CONFIG; 
		ruta += NAME_FILE_BF;
		ruta += snumero;
		ruta += EXTENSION_BITFIELD;
		bitfieldFile.open(ruta.c_str(), std::fstream::out);
		
		if(bitfieldFile.is_open()) {

			auxiliar= bitfield->getLength();
			bitfieldFile.write((const char*)&auxiliar,sizeof(uint32_t));
			bitfieldFile.write(bitfield->getData(), bitfield->getBytesLength());		
			bitfieldFile.close();
			resultado= true;
		}
		auxiliar= (*it)->getName().length();
		config.write((const char*)&auxiliar,sizeof(uint32_t));
		config.write((*it)->getName().c_str(), auxiliar);
		auxiliar= ruta.length();
		config.write((const char*)&auxiliar,sizeof(uint32_t));
		config.write(ruta.c_str(), auxiliar);
		auxiliar= this->obtenerOrden((*it)->getName());
		config.write((const char*)&auxiliar, sizeof(uint32_t));
	}
	config.close();		

	return resultado;
}

/*--------------------------------------------------------------------------*/
bool Controlador::cargarConfig() {
	
	bool resultado= false;
	std::fstream config;
			
	std::fstream bitFieldFile;
	std::string pathTorrent;
	config.open(NAME_FILE_CONFIG , std::fstream::in);
	
	config.seekg(0, std::fstream::end);
	uint32_t last= config.tellg();
	config.seekg(0, std::fstream::beg);
		
	if(config.is_open()) {
		resultado=true;
		char* cantTorrents= new char[sizeof(uint32_t)];
		config.read(cantTorrents, sizeof(uint32_t));
		uint32_t cantListaTorrents= *cantTorrents;
		
		for(uint32_t i= 0; i < cantListaTorrents; i++) {
			listaOrdenada.push_back("-");
		}

		while(last != config.tellg()){
			
			char* longNombreTorrent= new char[sizeof(uint32_t)];
			config.read(longNombreTorrent, sizeof(uint32_t));
			uint32_t longitudNombre= *longNombreTorrent;

			char* nombreTorrent= new char[longitudNombre+1];
			config.read(nombreTorrent,longitudNombre);
			nombreTorrent[longitudNombre] = '\0';
			
			char* longRuta= new char[sizeof(uint32_t)];
			config.read(longRuta, sizeof(uint32_t));
			uint32_t longitudRuta= *longRuta;
			
			char* ruta= new char[longitudRuta+1];
			config.read(ruta,longitudRuta);
			ruta[longitudRuta] = '\0';

			bitFieldFile.open(ruta,std::fstream::in);
			if(bitFieldFile.is_open()){
				pathTorrent= ""; 
				pathTorrent += PATH_CONFIG;
				pathTorrent += nombreTorrent;
				pathTorrent += "\0";
				
 				char* lengthBitField= new char[sizeof(uint32_t)];
 				bitFieldFile.read(lengthBitField, sizeof(uint32_t));
 				uint32_t length= *((uint32_t*)lengthBitField);
				
				BitField* bitField= new BitField(length);
				uint32_t lengthBytes = bitField->getBytesLength();
 				
 				char* dataBitField = new char[bitField->getBytesLength()+1];
 				bitFieldFile.read(dataBitField, lengthBytes);
 				dataBitField[lengthBytes] = '\0';
								
				bitField->setData(dataBitField);
				cliente->addTorrent(pathTorrent.c_str(), bitField);
				
				resultado = true;
				delete[] lengthBitField;
				bitFieldFile.close();	
			}
			
			delete[] longNombreTorrent;
			delete[] longRuta;
			delete[] ruta;
			
			char* orden = new char[sizeof(uint32_t)];
			config.read(orden,sizeof(uint32_t));
			uint32_t ordenVista = *orden;
			
			std::string filename= nombreTorrent;
			listaOrdenada[ordenVista]= nombreTorrent;
		}
	} 		
	
	config.close();
	return resultado;
}

/*--------------------------------------------------------------------------*/
Torrent* Controlador::obtenerTorrent(std::string filename) {
	
	const std::list<Torrent*>* listaTorrents= this->cliente->getListaTorrents();
	std::list<Torrent*>::const_iterator it;
	bool encontrado= false;
	
	for(it= listaTorrents->begin(); it != listaTorrents->end() && !encontrado; it++) {

	     if(filename ==  (*it)->getName()){
			encontrado= true;
			break;
	     }
	}	
	
	if(encontrado)
	     return *(it);
	return NULL;
}

/*--------------------------------------------------------------------------*/
/***Preparar Datos para mostrar en la vista****/
/*--------------------------------------------------------------------------*/
std::string intToString(uint32_t num) {
	
	std::string numS;
	std::stringstream cvz;
	cvz.width();
	cvz << num;
	numS= cvz.str();		
	
	return numS;
}

/*--------------------------------------------------------------------------*/
std::string Controlador::obtenerSize(uint64_t size) {
	
	int contador= 0;
	while((size/1024) > 0) {
		size= size/1024;
		contador++;
	}
	
	std::string sizeS;
	std::stringstream cvz;
	cvz.width();
	cvz << size;
	sizeS= cvz.str();
	
	switch(contador) {
		case 0: 	
			sizeS+= " b";
			break;
		case 1: 	
			sizeS+= " kb";
			break;
		case 2: 	
			sizeS+= " mb";
			break;
		case 3: 	
			sizeS+= " gb";
			break;		
		case 4:
			sizeS+= " tb";
			break;		
		default:
			break;					
	}
	
	return(sizeS);
}

/*--------------------------------------------------------------------------*/
std::string Controlador::obtenerDownloaded(double done) {
	
	std::string doneS;
	std::stringstream cvz;
	cvz.width();
	cvz << trunc(done);
	doneS= cvz.str();
	doneS+= " %";
	
	return(doneS);	
}

/*--------------------------------------------------------------------------*/
std::string Controlador::obtenerVelocidad(uint32_t length) {
	
	int contador= 0;
	while((length/1024) > 0) {
		length= length/1024;
		contador++;
	}
	
	std::string lengthS= intToString(length);
	
	switch(contador) {
		case 0: 	
			lengthS+= " b/s";
			break;
		case 1: 	
			lengthS+= " kb/s";
			break;
		case 2: 	
			lengthS+= " mb/s";
			break;
		default:
			break;					
	}	

	return(lengthS);	
}

/*--------------------------------------------------------------------------*/
std::string Controlador::obtenerETA(uint32_t time) {
	
	int segs= time%60;
	int mins= (time/60)%60;
	int hrs= (time/3600)%24;
	int dias= (time/3600)/24;
	
	std::string timeS;
	
	if(dias > 0) {
		timeS+= intToString(dias);
		timeS+= "d ";
	}
	
	if(hrs > 0) {
		timeS+= intToString(hrs);
		timeS+= "h ";
	}
	
	if(mins > 0) {
		timeS+= intToString(mins);
		timeS+= "m ";
	}

	if(segs > 0) {
		timeS+= intToString(segs);
		timeS+= "s ";
	}
	
	return(timeS);		
}

/*--------------------------------------------------------------------------*/
std::string Controlador::obtenerStatus(EstadoTorrent status) {
	
	std::string sStatus;
	
	switch(status) {
		case STOPPED: 
			sStatus= "Stopped";
		break;
		case PAUSED:
			sStatus= "Paused";
		break;	
		case DOWNLOADING:
			sStatus= "Downloading";
		break;
		case SEEDING:
			sStatus= "Seeding";
		break;
		case ERROR:
		break;	
	}
	
	return sStatus;			
}

/****************************************************************************/
