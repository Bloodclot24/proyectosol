#ifndef CONTROLADORSHELL_H_
#define CONTROLADORSHELL_H_

#include <stdlib.h>
#include "../../Controlador/controlador.h"
#include "../../Vista/Shell/shell.h"

/****************************************************************************/
class ControladorShell: public Controlador {
	
	private:
		Shell* shell;
		
		bool validarNumFile(std::string numFile);
		uint32_t contadorOrden;
		int cantFiles;
		const char* obtenerFilename(std::string numFile);
		
	public:
		ControladorShell(); 
		virtual ~ControladorShell();
	
		virtual void correr();

		virtual bool addTorrent(std::string pathTorrent);
		virtual void addUrlTorrent();
		virtual void mostrarTrackers();
		virtual void mostrarFiles();
		virtual void mostrarGeneral(std::string numFile);	
		virtual void mostrarPeers(std::string numFile);
		virtual void mostrarPieces(std::string numFile);
		virtual void removeFile(std::string numFile);
		virtual void startFile(std::string numFile);
		virtual void pauseFile(std::string numFile);
		virtual void stopFile(std::string numFile);
		
		virtual uint32_t obtenerOrden(std::string filename);
		virtual void insertarOrdenado(std::string filename, uint32_t orden) {};
	
	/* MODELO -> VISTA 
	 * =============== */		 
	/*Message*/
	virtual void agregarMessage(std::string message);
};

/****************************************************************************/
#endif /*CONTROLADORSHELL_H_*/
class ControladorShell;
