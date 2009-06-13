#include "controlador.h"

/****************************************************************************/
Controlador::Controlador(): cliente() {

	this->config.open(PATH_CONFIG, std::fstream::in);
	if(config.is_open()) {
		//levantar los datos
		//pasarselo al modelo y a la vista
		this->config.close();
	}
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
	this->config.open(PATH_CONFIG, std::fstream::out);
	
	if(config.is_open()) {
		//pedir al modelo datos sobre los archivos
		std::string datos;
		this->config.write(datos.c_str(), datos.length());
		this->config.close();
		resultado= true;
	}
	
	return resultado;
}


/****************************************************************************/
