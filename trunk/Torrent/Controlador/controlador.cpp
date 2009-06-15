#include "controlador.h"

/****************************************************************************/
Controlador::Controlador(): cliente() {

	FileManager::crearDirectorio(PATH_DOWNLOADS);
	FileManager::crearDirectorio(PATH_CONFIG);
	cargarConfig();
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
std::string Controlador::crearCopiaTorrent(std::string pathTorrent) {
	
	std::fstream file;
  	file.open(pathTorrent.c_str(), std::fstream::in);
	file.seekg(0, std::fstream::end);
	uint32_t last= file.tellg();
	file.seekg(0, std::fstream::beg);
	char* buffer= new char[last*sizeof(char)];
	file.read(buffer,last);
	std::fstream fileCopia;
	std::string ruta;
	ruta+= PATH_CONFIG;
	ruta+= FileManager::obtenerFilename(pathTorrent);
	fileCopia.open(ruta.c_str(), std::fstream::out);
  	fileCopia.write(buffer, last); 
  	file.close();
  	fileCopia.close();
  	
  	return ruta;
}

/*--------------------------------------------------------------------------*/
bool Controlador::guardarConfig() {
	
	std::cout << "ACA TOY\n";

	bool resultado= false;
	std::fstream bitfieldFile;
	std::fstream config;
	uint32_t contador = 0;
	std::string snumero;
	uint32_t auxiliar;	
    std::stringstream cvz;
	std::list<Torrent*>* listaTorrents  = this->cliente.getListaTorrents();
	std::list<Torrent*>::iterator it;
	config.open(NAME_FILE_CONFIG , std::fstream::out);
	std::cout << "SE CREO CONFIG\n";
//	if(!bitfieldFile.is_open()) resultado = false;
//	if(resultado){ 
		for(it = listaTorrents->begin(); it != listaTorrents->end(); it++,contador++){
			std::cout << "LA LISTA NO ESTA VACIAAAAAAAAAAAAA\n";
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
			std::cout << "Se va a escribir long torrent en config "<<auxiliar<<"\n";
			config.write((const char*)&auxiliar,sizeof(uint32_t));
			std::cout << "Se va a escribir nombre torrent en config "<<(*it)->getName().c_str()<<"\n";
			config.write((*it)->getName().c_str(), auxiliar);
			auxiliar= ruta.length();
			std::cout << "Se va a escribir long ruta en config "<<auxiliar<<"\n";
			config.write((const char*)&auxiliar,sizeof(uint32_t));
			std::cout << "Se va a escribir ruta en config "<<ruta.c_str()<<"\n";
			config.write(ruta.c_str(), auxiliar);
			auxiliar= obtenerOrden((*it)->getName());
			std::cout << "Se va a escribir orden en config "<<auxiliar<<"\n";
			config.write((const char*)&auxiliar, sizeof(uint32_t));
			config.close();
		}
		
//	}
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
		std::cout << "Se abrio el config\n";
		resultado=true;
		while(config.tellg() != std::fstream::end){
			std::cout << "Se leyo del config\n";
			char* longNombreTorrent = new char[sizeof(uint32_t)*sizeof(char)];
			config.read(longNombreTorrent,sizeof(uint32_t));
			uint32_t longitudNombre = atoi(longNombreTorrent);
			std::cout << "Se leyo la long del nombre torrent "<<longNombreTorrent<<"\n";
			std::cout << "Se leyo la long del nombre torrent "<<longitudNombre<<"\n";
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
				cliente.addTorrent(pathTorrent.c_str());
				char* tamanioBitField = new char[sizeof(uint32_t)*sizeof(char)];
				bitFieldFile.read(tamanioBitField, sizeof(uint32_t));
				uint32_t tamanioBF = atoi(tamanioBitField);
				char* datosBitField = new char[tamanioBF*sizeof(char)];
				bitFieldFile.read(datosBitField,tamanioBF);
				//Pasarle datosBitField al torrent
				cliente.setBitFieldTorrent(nombreTorrent, datosBitField);
				resultado = true;
//				delete[] tamanioBitField;	
			}
//			delete[] longNombreTorrent;
//			delete[] longRuta;
//			delete[] ruta;
			
			char* orden = new char[sizeof(uint32_t)*sizeof(char)];
			config.read(orden,sizeof(uint32_t));
			uint32_t ordenVista = atoi(orden);
			//le paso a la vista el orden y el nombre del Torrent
			std::string filename(nombreTorrent,longitudNombre);
			insertarOrdenado(filename,ordenVista);
//			delete[] nombreTorrent;
		}
		std::cout << "ERROR AL ABRIR EL CONFIG "<<std::endl;
	}
	return resultado;
}

/*--------------------------------------------------------------------------*/
Controlador::~Controlador() {
	
	std::cout << "ACA TOY destructor\n";
	
	guardarConfig();
	
}

/****************************************************************************/
