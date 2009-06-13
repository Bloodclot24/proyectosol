#ifndef CONTROLADORVISTA_H_
#define CONTROLADORVISTA_H_

#include "../../Controlador/controlador.h"
#include "../../Vista/GUI/vistaTorrent.h"

/****************************************************************************/
class ControladorGUI: public Controlador {

	private:
		VistaTorrent* vista;	
		
		int all;
		int downloading;
		int completed;
		int pause;
		int active;
		void actualizarCantActividades();
		
	public:
		ControladorGUI(); 
		virtual ~ControladorGUI();

		virtual void correr();
		
		/*Files*/
		virtual bool addTorrent(std::string pathTorrent);
		virtual void addUrlTorrent();
		virtual void removeFile(std::string file);
		virtual void startFile(std::string file);
		virtual void pauseFile(std::string file);
		virtual void stopFile(std::string file);
		
		virtual void actualizarDone(std::string file, int piece, int done);
		virtual void actualizarStatus(std::string file, int piece, 
		                              std::string status);
		virtual void actualizarDownSpeed(std::string file, int piece, 
		                                 std::string downSpeed);
		virtual void actualizarUpSpeed(std::string file, int piece, 
		                               std::string upSpeed);	
		
		/*Trackers*/
		virtual void modificarStatusTracker(std::string name, std::string status);		
		virtual void agregarSeedTracker(std::string name, int seed);
		virtual void eliminarTracker(std::string name);
		
		/*Todas las Pestanias*/
		virtual void actualizarPestanias(std::string filename);
		
		virtual void exit();		
};

/****************************************************************************/
#endif /*CONTROLADORVISTA_H_*/
class ControladorVista;