#ifndef CONTROLADORSHELL_H_
#define CONTROLADORSHELL_H_

#include "controlador.h"
#include "../Modelo/Shell/shell.h"

/****************************************************************************/
class ControladorShell: public Controlador {
	
	private:
		Shell* shell;
		
	public:
		ControladorShell(); 
		virtual ~ControladorShell();
	
		virtual void correr();

		virtual void mostrarGeneral();	
		virtual void mostrarTrackers();
		virtual void mostrarPeers();
		virtual void mostrarPieces(std::string numFile);
		virtual void mostrarAll();
		virtual void mostrarDownloading();
		virtual void mostrarCompleted();
		virtual void mostrarActive();
		virtual void mostrarInactive();
		virtual void mostrarAddTorrent();
		virtual void mostrarAddUrlTorrent();
		
		virtual void removeFile(std::string numFile);
		virtual void startFile(std::string numFile);
		virtual void pauseFile(std::string numFile);
		virtual void stopFile(std::string numFile);
		virtual void exit() {};
};

/****************************************************************************/
#endif /*CONTROLADORSHELL_H_*/
class ControladorShell;