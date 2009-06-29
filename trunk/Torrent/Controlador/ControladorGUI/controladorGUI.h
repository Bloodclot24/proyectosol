#ifndef CONTROLADORVISTA_H_
#define CONTROLADORVISTA_H_

#include "../../Controlador/controlador.h"
#include "../../Vista/GUI/vistaTorrent.h"

/****************************************************************************/
class ControladorGUI: public Controlador {

	private:
		VistaTorrent* vista;
		RefreshVista* refrescador;
		
		int all;
		int downloading;
		int completed;
		int active;
		void actualizarCantActividades();
	
	protected:
		virtual void mostrarAnnounceUrlTorrent(Torrent* torrent);
		
	public:
		ControladorGUI(); 
		virtual ~ControladorGUI();

		virtual void correr();
		
		/* VISTA -> MODELO 
		 * =============== */
		/*Files*/
		virtual void mostrarFiles();
		virtual bool addTorrent(std::string pathTorrent);
		virtual void addUrlTorrent();
		virtual void removeFile(std::string file);
		virtual void startFile(std::string file);
		virtual void pauseFile(std::string file);
		virtual void stopFile(std::string file);
		
		/*Trackers*/
		virtual void mostrarTrackers();
		
		/*Todas las Pestanias*/
		virtual void actualizarPestanias(std::string filename);
		
		virtual uint32_t obtenerOrden(std::string filename);
		virtual void insertarOrdenado(std::string filename, uint32_t orden) {};
					
		/*Refrescador*/		
		virtual void refrescar();
		
		/* DialogDelay */
		virtual void mostrarDialogDelay();
		virtual void esconderDialogDelay();
		
		/* MODELO -> VISTA 
		 * =============== */		 
		/*Files*/ 		 
		virtual void start(std::string filename);
		virtual void pause(std::string filename);
		virtual void stop(std::string filename);		
		virtual void actualizarDone(std::string file, int piece, int done);
		virtual void actualizarStatus(std::string file, int piece, 
		                              std::string status);
		virtual void actualizarDownSpeed(std::string file, int piece, 
		                                 std::string downSpeed);
		virtual void actualizarUpSpeed(std::string file, int piece, 
		                               std::string upSpeed);
		virtual void actualizarTime(std::string file, std::string time);	
		
		/*Trackers*/
		virtual void modificarStatusTracker(std::string name, 
		                                    std::string status);		
		virtual void agregarSeedTracker(std::string name, int seed);
		virtual void eliminarTracker(std::string name);
		
		/*Message*/
		virtual void agregarMessage(std::string message);
};

/****************************************************************************/
#endif /*CONTROLADORVISTA_H_*/
class ControladorVista;
