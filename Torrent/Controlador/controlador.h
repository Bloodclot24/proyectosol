#ifndef CONTROLADOR_H_
#define CONTROLADOR_H_

#include <iostream>
#include <fstream>
#include "../Modelo/Client/client.h"
#include "../Modelo/Client/bitField.h"
#include "../Modelo/FileManager/fileManager.h"

#define EXTENSION ".torrent"
#define TAM_EXTENSION 8
#define PATH_DOWNLOADS  "Downloads"
#define PATH_CONFIG  "Downloads/Config/"
#define NAME_FILE_BF "Torrent"
#define EXTENSION_BITFIELD ".bf"
#define NAME_FILE_CONFIG "Downloads/Config/config.sun"
/****************************************************************************/
class Controlador {
	
	protected:
		bool validarExtensionFile(std::string path);
		Client cliente;
//		std::fstream config;	
		
		
		/*Persistencia*/
		bool guardarConfig();
		bool cargarConfig();
				
	public:
		Controlador();
		virtual ~Controlador() {};

		virtual void correr()= 0;
		
		/*Files*/
		virtual void mostrarFiles() {};

		virtual bool addTorrent(std::string pathTorrent)= 0;
		virtual void addUrlTorrent()= 0;

		virtual void removeFile(std::string file)= 0;
		virtual void startFile(std::string file)= 0;
		virtual void pauseFile(std::string file)= 0;
		virtual void stopFile(std::string file)= 0;
		
		virtual void actualizarDone(std::string file, int piece, int done) {};
		virtual void actualizarStatus(std::string file, int piece, 
		                              std::string status) {};
		virtual void actualizarDownSpeed(std::string file, int piece, 
		                                 std::string downSpeed) {};
		virtual void actualizarUpSpeed(std::string file, int piece, 
		                               std::string upSpeed) {};	

		/*General*/
		virtual void mostrarGeneral(std::string numfile) {};
		
		/*Trackers*/
		virtual void mostrarTrackers() {};
		virtual void modificarStatusTracker(std::string name,
		                                    std::string status) {};		
		virtual void agregarSeedTracker(std::string name, int seed) {};
		virtual void eliminarTracker(std::string name) {};
		
		/*Peers*/
		virtual void mostrarPeers(std::string numfile) {};
		
		/*Pieces*/
		virtual void mostrarPieces(std::string numFile) {};
		
		/*Todas las Pestanias*/
		virtual void actualizarPestanias(std::string filename) {};

		/*Orden*/
		virtual uint32_t obtenerOrden(std::string filename)= 0;

		virtual void exit()= 0;
		
};

/****************************************************************************/
#endif /*CONTROLADOR_H_*/
