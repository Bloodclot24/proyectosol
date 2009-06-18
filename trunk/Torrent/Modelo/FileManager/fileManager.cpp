#include "fileManager.h"

FileManager::FileManager(const char* filename,uint32_t tamanio) {

     std::cout << "CREANDO EL ARCHIVO: " << filename << "\n";
	this->file.open(filename,std::fstream::in);
	if(!this->file.is_open()){
		this->file.clear();
		this->file.open(filename,std::fstream::out);
		if(!this->file.is_open()) this->status = -1;
		else{
			for(uint32_t size = 0; size < tamanio; size++){
				this->file.put('\0');
			}
			this->status = 0;
		}
	}else{
		this->file.close();
		this->status = 1;
	}
	this->file.open(filename,std::fstream::in | std::fstream::out);

}

int FileManager::getStatus(){
	return this->status;
}

bool FileManager::agregarPieza(const char* pieza, uint32_t offset, uint32_t tamanio)
{

     std::cout << "Se escriben los datos: offset->" << offset << " | tamanio-> " << tamanio << " | Archivo-> " << this << std::endl;
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

std::string FileManager::obtenerFilename(std::string path) {
	
	size_t pos= path.find_last_of(COMIENZO_FILENAME, path.length());

	std::string filename;
	if(pos != std::string::npos) {
		filename.assign(path, pos+1, path.length()-pos-1);	
		return filename;
	} else
		return path;
}
		
FileManager::~FileManager() {
	
	this->file.close();
}
