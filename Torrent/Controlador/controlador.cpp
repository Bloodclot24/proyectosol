#include "controlador.h"

/****************************************************************************/
Controlador::Controlador(): cliente() {

//	this->config.open(PATH_CONFIG, std::fstream::in);
//	if(config.is_open()) {
//		//levantar los datos
//		//pasarselo al modelo y a la vista
//		this->config.close();
//	}
}

/*--------------------------------------------------------------------------*/
bool Controlador::validarExtensionFile(std::string path) {
	
	size_t pos= path.find(EXTENSION, path.length()-TAM_EXTENSION);
	
	if(pos != std::string::npos)
		return true;
	else
		return false; 
}

/*--------------------------------------------------------------------------*/
bool Controlador::guardarConfig() {
	
	bool resultado= false;
	std::fstream bitfieldFile;
	std::fstream config;
	uint32_t contador = 0;
	std::string snumero;
	uint32_t auxiliar;	
    std::stringstream cvz;
	FileManager::crearDirectorio(PATH_DOWNLOADS);
	FileManager::crearDirectorio(PATH_CONFIG);
	std::list<Torrent*>* listaTorrents  = this->cliente.getListaTorrents();
	std::list<Torrent*>::iterator it;
	config.open(NAME_FILE_CONFIG , std::fstream::out);
	if(!bitfieldFile.is_open()) resultado = false;
	if(resultado){ 
		for(it = listaTorrents->begin(); it != listaTorrents->end(); it++,contador++){
			BitField* bitfield = (*it)->getBitField();
			char* data = bitfield->getData();
	   		cvz <<  contador;
     		snumero = cvz.str();
     		std::string ruta;
			ruta += PATH_CONFIG; 
			ruta += NAME_FILE_BF;
			ruta += snumero;
			ruta += EXTENSION_BITFIELD;
			bitfieldFile.open(ruta.c_str(), std::fstream::out);
			if(bitfieldFile.is_open()) {
				auxiliar = bitfield->getBytesLength();
				bitfieldFile.write((const char*)&auxiliar,sizeof(uint32_t));
				bitfieldFile.write(data, strlen(data));		
				bitfieldFile.close();
				resultado= true;
			}
			auxiliar= (*it)->getName().length();
			config.write((const char*)&auxiliar,sizeof(uint32_t));
			config.write((*it)->getName().c_str(), auxiliar);
			auxiliar= ruta.length();
			config.write((const char*)&auxiliar,sizeof(uint32_t));
			config.write(ruta.c_str(), auxiliar);
			auxiliar= obtenerOrden((*it)->getName());
			config.write((const char*)&auxiliar, sizeof(uint32_t));
			config.close();
		}
		
	}
	
	
	
	return resultado;
}

/*--------------------------------------------------------------------------*/
bool Controlador::cargarConfig() {
	bool resultado = false;
	std::fstream config;
	std::fstream bitFieldFile;
	std::string pathTorrent;
	config.open(NAME_FILE_CONFIG , std::fstream::in);
	if(config.is_open()){
		resultado=true;
		char* longNombreTorrent = new char[sizeof(uint32_t)];
		config.read(longNombreTorrent,sizeof(uint32_t));
		uint32_t longitudNombre = atoi(longNombreTorrent);
		char* nombreTorrent = new char[longitudNombre];
		config.read(nombreTorrent,longitudNombre);
		char* longRuta = new char[sizeof(uint32_t)];
		config.read(longRuta,sizeof(uint32_t));
		uint32_t longitudRuta = atoi(longRuta);
		char* ruta = new char[longitudRuta];
		config.read(ruta,longitudRuta);
		bitFieldFile.open(ruta,std::fstream::in);
		if(!bitFieldFile.is_open()){
			pathTorrent += PATH_CONFIG;
			pathTorrent += nombreTorrent;
			addTorrent(pathTorrent);
				
		}
		
	}
	return resultado;
}

/****************************************************************************/
