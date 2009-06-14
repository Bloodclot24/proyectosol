#include "fileManager.h"

FileManager::FileManager(const char* filename,uint32_t tamanio) {

     std::cout << "CREANDO EL ARCHIVO: " << filename << "\n";
	this->file.open(filename,std::fstream::in);
	if(!this->file.is_open()){
		this->file.open(filename,std::fstream::out);
		
		for(uint32_t size = 0; size < tamanio; size++){
			this->file.put('\0');
		}
	}else{
		this->file.close();
	}
	this->file.open(filename,std::fstream::in | std::fstream::out);

}

bool FileManager::agregarPieza(const char* pieza, uint32_t offset, uint32_t tamanio)
{

     std::cout << "Se escriben los datos\n";
	bool resultado = false;
	this->file.seekp(offset,std::ios_base::beg);
	if(this->file.good()){
		this->file.write(pieza,tamanio);
		if(this->file.good()){
			resultado = true;
		}
	}
	this->file.clear();
	return resultado;
}

bool FileManager::obtenerPieza(char* pieza, uint32_t offset, uint32_t tamanio)
{
	bool resultado = false;
	this->file.seekg(offset,std::ios_base::beg);
	if(this->file.good()){
		this->file.read(pieza,tamanio);
		if(this->file.good()){
			resultado = true;
		}
	}
	this->file.clear();
	return resultado;
}
		
int FileManager::crearDirectorio(const char* dirname) {
	
	int estado= mkdir(dirname, PERMISO);
	
	if(estado < 0)
		return(errno);
	else
		return 0;
} 
		
FileManager::~FileManager() {
	
	this->file.close();
}
