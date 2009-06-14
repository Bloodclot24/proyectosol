#ifndef FILEMANAGER_H_
#define FILEMANAGER_H_

#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define PERMISO 0700

class FileManager {
	
	private:
		std::fstream file;
			
	public:	
		FileManager(const char* filename,uint32_t tamanio);
		bool agregarPieza(const char* pieza, uint32_t offset, uint32_t tamanio);
		bool obtenerPieza(char* pieza, uint32_t offset, uint32_t tamanio);
		static int crearDirectorio(const char* dirname); 
		~FileManager();
};

#endif /*FILEMANAGER_H_*/
