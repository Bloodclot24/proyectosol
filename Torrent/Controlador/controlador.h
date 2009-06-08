#ifndef CONTROLADOR_H_
#define CONTROLADOR_H_

#include "../Modelo/Shell/shell.h"

/****************************************************************************/
class Controlador {
	
	private:
		static Controlador	controlador;
		Shell* shell;
		
		Controlador();
	
	public: 
		static Controlador* getInstancia();

		void correrConsola();

		void mostrarGeneral();	
		void mostrarTrackers();
		void mostrarPeers();
		void mostrarPieces(std::string numFile);
		void mostrarAll();
		void mostrarDownloading();
		void mostrarCompleted();
		void mostrarActive();
		void mostrarInactive();
		void mostrarAddTorrent();
		void mostrarAddUrlTorrent();
		
		void removeFile(std::string numFile);
		void startFile(std::string numFile);
		void pauseFile(std::string numFile);
		void stopFile(std::string numFile);
};

/****************************************************************************/
#endif /*CONTROLADOR_H_*/
