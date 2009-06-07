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

		void mostrarGeneral();	
		void mostrarTrackers();
		void mostrarPeers();
		void mostrarPieces();
		void mostrarAll();
		void mostrarDownloading();
		void mostrarCompleted();
		void mostrarActive();
		void mostrarInactive();
		void mostrarAddTorrent();
		void mostrarAddUrlTorrent();
		
};

/****************************************************************************/
#endif /*CONTROLADOR_H_*/
class Controlador;