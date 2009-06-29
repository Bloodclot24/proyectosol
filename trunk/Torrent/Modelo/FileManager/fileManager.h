#ifndef FILEMANAGER_H_
#define FILEMANAGER_H_

#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define PERMISO 0700
#define COMIENZO_FILENAME "/"

/****************************************************************************/
class FileManager {
	
	private:
		std::fstream file;
		int status;
			
	public:
	    /**
		 * Crea un FileManager.
		 */	
		FileManager(const char* filename,uint32_t tamanio);
		
		/**
		 * Destruye el FileManager.
         */
		~FileManager();
		
		/**
		 * Devuelve el estado del FileManager.
		 *      
         * @return El estado del FileManager.
         */
		int getStatus();

		/**
		 * Agrega una pieza en el archivo del FileManager.
         * 
         * @param pieza Pieza a agregar.
		 * @param offset Offset a partir del cual se agregara la pieza.
         * @param tamanio Tamanio de la pieza agregar.
         * 
         * @return Devuelve true si pudo agregar la pieza, caso 
         * contrario false.
         */
		bool agregarPieza(const char* pieza, uint32_t offset, uint32_t tamanio);

		/**
		 * Devuelve una pieza del archivo del FileManager.
         * 
         * @param pieza Buffer para devolver la pieza.
		 * @param offset Offset a partir del cual se obtendra la pieza.
         * @param tamanio Tamanio de la pieza a devolver.
         * 
         * @return Devuelve true si pudo obtener la pieza, caso 
         * contrario false.
         */	
		bool obtenerPieza(char* pieza, uint32_t offset, uint32_t tamanio);

		/**
		 * Crea un directorio.
         * 
         * @param dirname Directorio a crear.
         * 
         * @return Devuelve 0 si se pudo crear el directorio, caso 
         * contrario -1.
         */
		static int crearDirectorio(const char* dirname);

		/**
		 * Dado un path de un archivo, lo parsea y devuelve el nombre del
		 * archivo junto con su extension.
         * 
         * @param path Path del archivo a obtener el filename.
         * 
         * @return El filename obtenido.
         */
		static std::string obtenerFilename(std::string path); 
};

/****************************************************************************/
#endif /*FILEMANAGER_H_*/
