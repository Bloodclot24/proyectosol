#include "fileManager.h"
#include <string.h>

int main(int argc, char** argv){
	
	FileManager fileManager("pruebaFileManager",100);
	const char* pieza = "Hola mundo";
	bool resultado;
	resultado =fileManager.agregarPieza(pieza,55,strlen(pieza));
	if(resultado){
		std::cout<< "Se agrego la pieza al archivo"<<std::endl;
	}else std::cout<< "Error al agregar la pieza al archivo"<<std::endl;
	resultado =fileManager.agregarPieza(pieza,105,strlen(pieza));
	if(resultado){
		std::cout<< "Se agrego la pieza al archivo"<<std::endl;
	}else std::cout<< "Error al agregar la pieza al archivo"<<std::endl;
	char piezaObtenida[strlen(pieza)+1];
	resultado = fileManager.obtenerPieza(piezaObtenida,55,strlen(pieza)+1);
	if(resultado){
		std::cout<< "Se obtuvo la pieza al archivo"<<std::endl;
		std::cout <<"Pieza obtenida: "<<piezaObtenida<<std::endl;
	}else std::cout<< "Error al obtener la pieza al archivo"<<std::endl;
	resultado = fileManager.obtenerPieza(piezaObtenida,105,strlen(pieza)+1);
	if(resultado){
		std::cout<< "Se obtuvo la pieza al archivo"<<std::endl;
		std::cout <<"Pieza obtenida: "<<piezaObtenida<<std::endl;
	}else std::cout<< "Error al obtener la pieza al archivo"<<std::endl;
	
	return 0;
	
}