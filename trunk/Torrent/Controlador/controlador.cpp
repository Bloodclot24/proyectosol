#include "controlador.h"

/****************************************************************************/
Controlador::Controlador(): cliente() {

	FileManager::crearDirectorio(PATH_DOWNLOADS);
	FileManager::crearDirectorio(PATH_CONFIG);
	this->refrescador= new RefreshVista(this);
	this->refrescador->start();
}

/*--------------------------------------------------------------------------*/
Controlador::~Controlador() {

	this->refrescador->stop();	
};

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
	if(!bitfieldFile.is_open()) resultado = false;

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
		std::cout << "Se va a escribir long torrent en config "<<auxiliar<<"\n";
		config.write((const char*)&auxiliar,sizeof(uint32_t));
		std::cout << "Se va a escribir nombre torrent en config "<<(*it)->getName().c_str()<<"\n";
		config.write((*it)->getName().c_str(), auxiliar);
		auxiliar= ruta.length();
		std::cout << "Se va a escribir long ruta en config "<<auxiliar<<"\n";
		config.write((const char*)&auxiliar,sizeof(uint32_t));
		std::cout << "Se va a escribir ruta en config "<<ruta.c_str()<<"\n";
		config.write(ruta.c_str(), auxiliar);
		auxiliar= this->obtenerOrden((*it)->getName());
		std::cout << "Se va a escribir orden en config "<<auxiliar<<"\n";
		config.write((const char*)&auxiliar, sizeof(uint32_t));
		}
	config.close();		

	return resultado;
}

/*--------------------------------------------------------------------------*/
bool Controlador::cargarConfig() {
	
	bool resultado = false;
	std::fstream config;
	std::fstream bitFieldFile;
	std::string pathTorrent;
	config.open(NAME_FILE_CONFIG , std::fstream::in);
	
	config.seekg(0, std::fstream::end);
	uint32_t last= config.tellg();
	config.seekg(0, std::fstream::beg);
		
	if(config.is_open()) {
		std::cout << "Se abrio el config\n";
		resultado=true;
		
		while(last != config.tellg()){
			
			std::cout << "Se leyo del config\n";
			
			char* longNombreTorrent= new char[sizeof(uint32_t)];
			config.read(longNombreTorrent, sizeof(uint32_t));
			uint32_t longitudNombre= *longNombreTorrent;
			
			std::cout << "long del nombre torrent: "<<longitudNombre<<"\n";
			
			
			std::cout << "long del nombre torrent: "<<longNombreTorrent<<"\n";
			char* nombreTorrent= new char[longitudNombre+1];
			config.read(nombreTorrent,longitudNombre);
			nombreTorrent[longitudNombre] = '\0';
			
			std::cout << "nombre torrent: "<< nombreTorrent <<"\n";
			
			
			char* longRuta= new char[sizeof(uint32_t)];
			config.read(longRuta, sizeof(uint32_t));
			uint32_t longitudRuta= *longRuta;
			
			std::cout << "long de la ruta : "<<longitudRuta<<"\n";
			
			char* ruta= new char[longitudRuta+1];
			config.read(ruta,longitudRuta);
			ruta[longitudRuta] = '\0';
			std::cout << "ruta torrent: "<< ruta <<"\n";


			bitFieldFile.open(ruta,std::fstream::in);
			if(bitFieldFile.is_open()){
				pathTorrent= ""; 
				pathTorrent += PATH_CONFIG;
				pathTorrent += nombreTorrent;
				pathTorrent += "\0";
				
				std::cout << "Ruta Torrent: "<< ruta <<"\n";
				
				std::cout<<"Esperando addTorrent: \n";
				

				
				
				std::cout<<"Se realizo addTorrent: \n";
				//char* tamanioBitField = new char[sizeof(uint32_t)];
				//bitFieldFile.read(tamanioBitField, sizeof(uint32_t));
				//uint32_t tamanioBF = *tamanioBitField;
				//char* datosBitField = new char[tamanioBF];
				//bitFieldFile.read(datosBitField,tamanioBF);
				//datosBitField[tamanioBF] = '\0';
				
				
				//TODO: esto no va mas
				//cliente.setBitFieldTorrent(nombreTorrent, datosBitField);
				//Aca va el nuevo addTorrent
				cliente.addTorrent(pathTorrent.c_str());
				//cliente.addTorrent(pathTorrent.c_str(), datosBitField);

				
				resultado = true;
				//delete[] tamanioBitField;
				bitFieldFile.close();	
			}
			
			delete[] longNombreTorrent;
			delete[] longRuta;
			delete[] ruta;
			
			char* orden = new char[sizeof(uint32_t)];
			config.read(orden,sizeof(uint32_t));
			uint32_t ordenVista = *orden;
			std::string filename(nombreTorrent,longitudNombre);
			
//			insertarOrdenado(filename,ordenVista);

			std::cout << "orden: "<< ordenVista <<"\n";

			delete[] nombreTorrent;
			
			
			std::cout << "-------------------------" <<"\n";
						
		}
		
		std::cout << "LLEGO ACA" <<"\n";

	} 		
	
	config.close();
	return resultado;
}

/*--------------------------------------------------------------------------*/
void Controlador::refrescar() {
	
	refrescador->refrescar();
}

/*--------------------------------------------------------------------------*/
std::string Controlador::getEstadoTorrent(EstadoTorrent estado) {
	
	std::string sEstado;
	
	switch(estado) {
		case STOPPED: 
			sEstado= "Stopped";
		break;
		case PAUSED:
			sEstado= "Paused";
		break;	
		case DOWNLOADING:
			sEstado= "Downloading";
		break;
		case SEEDING:
			sEstado= "Seeding";
		break;
		case ERROR:
		break;	
	}
	
	return sEstado;			
}

/*--------------------------------------------------------------------------*/
Torrent* Controlador::obtenerTorrent(std::string filename) {
	
	std::list<Torrent*>* listaTorrents= this->cliente.getListaTorrents();
	std::list<Torrent*>::iterator it;
	bool encontrado= false;
	
	for(it= listaTorrents->begin(); it != listaTorrents->end() && !encontrado; it++) {

		if(filename ==  (*it)->getName())
			encontrado= true;
	}	
	
	return *(--it);
}

/****************************************************************************/
