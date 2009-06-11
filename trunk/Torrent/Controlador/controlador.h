#ifndef CONTROLADOR_H_
#define CONTROLADOR_H_

#include <iostream>

#define EXTENSION ".torrent"
#define TAM_EXTENSION 8

/****************************************************************************/
class Controlador {
	
	protected:
		bool validarExtensionFile(std::string path);
	
	public:
		virtual ~Controlador() {};

		virtual void correr()= 0;
		
		/*Files*/
		virtual void mostrarAll() {};
		virtual void mostrarDownloading() {};
		virtual void mostrarCompleted() {};
		virtual void mostrarActive() {};
		virtual void mostrarInactive() {};
	
		virtual void mostrarAddTorrent() {};
		virtual void mostrarAddUrlTorrent() {};
		virtual bool addTorrent(std::string pathTorrent) { return false;};
		virtual void addUrlTorrent() {};

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
		virtual void mostrarGeneral() {};
		
		/*Trackers*/
		virtual void mostrarTrackers() {};
		virtual void modificarStatusTracker(std::string name,
		                                    std::string status) {};		
		virtual void agregarSeedTracker(std::string name, int seed) {};
		virtual void eliminarTracker(std::string name) {};
		
		/*Peers*/
		virtual void mostrarPeers() {};
		
		/*Pieces*/
		virtual void mostrarPieces(std::string numFile) {};
		
		/*Todas las Pestanias*/
		virtual void actualizarPestanias(std::string filename) {};

		virtual void exit()= 0;
};

/****************************************************************************/
#endif /*CONTROLADOR_H_*/