#ifndef CONTROLADORGUI_H_
#define CONTROLADORGUI_H_

#include "../../Controlador/controlador.h"
#include "../../Vista/GUI/vistaTorrent.h"
class ControladorGUI;
#include "../../Vista/GUI/refrescador.h"

/****************************************************************************/
/**
 * El controlador de la vista de la aplicacion Sun Torrent.
 */
class ControladorGUI: public Controlador {

	private:
		VistaTorrent* vista;
		Refrescador* refrescador;
		
		int all;
		int downloading;
		int completed;
		int active;
		void actualizarCantActividades();

		void mostrarFile(Torrent* torrent);
		void modificarGeneral(Torrent* torrent);
		void modificarPeers(Torrent* torrent);
			
	protected:
		virtual void mostrarAnnounceUrlTorrent(Torrent* torrent);
		
	public:
		/**
		 * Crea un controladorGUI. 
		 */
		ControladorGUI(); 
		
		/**
		 * Destruye el controladorGUI. 
		 */
		virtual ~ControladorGUI();

		/**
		 * Comienza a correr la aplicacion, tanto el modelo como el GUI. 
		 */
		virtual void correr();
		
		/* VISTA -> MODELO 
		 * =============== */
		/********Files********/
		/**
		 * Muestra todos los archivos en el panel de transferencias. 
		 */
		virtual void mostrarFiles();
		
		/**
		 * Agrega una descarga a partir de un archivo .torrent.
		 * 
		 * @param pathTorrent Directorio del archivo .torrent. 
		 */
		virtual bool addTorrent(std::string pathTorrent);
		
		/**
		 * Agrega una descarga a partir de una URL.
		 * 
		 * @param pathTorrent Directorio del archivo .torrent. 
		 */
		virtual void addUrlTorrent();
		
		/**
		 * Elimina una descarga pasada por parametro.
		 * 
		 * @param file Nombre del archivo a eliminar. 
		 */
		virtual void removeFile(std::string file);
	
		/**
		 * Comienza una descarga pasada por parametro.
		 * 
		 * @param file Nombre del archivo a bajar. 
		 */	
		virtual void startFile(std::string file);
		
		/**
		 * Pausa una descarga pasada por parametro.
		 * 
		 * @param file Nombre del archivo a pausar. 
		 */
		virtual void pauseFile(std::string file);
		
		/**
		 * Detiene una descarga pasada por parametro.
		 * 
		 * @param file Nombre del archivo a detener. 
		 */
		virtual void stopFile(std::string file);
		
		/********Trackers********/
		/**
		 * Muestra todos los trackers. 
		 */
		virtual void mostrarTrackers();
		
		/********Todas las Pestanias********/
		/**
		 * Actualiza las pestanias teniendo en cuenta la fila seleccionada. 
		 * 
		 * @param filename Nombre del archivo de la fila seleccionada.
		 * con sus detalles las pestanias. 
		 */
		virtual void actualizarPestanias(std::string filename);
		
		/**
		 * Limpia las pestania peers y general. 
		 */
		virtual void limpiarPestanias();
		
		/********Orden********/
		/**
		 * Obtiene el orden del archivo pasado por parametro.
		 * 
		 * @param filename Nombre del archivo a buscar el orden.
		 */ 
		virtual uint32_t obtenerOrden(std::string filename);
			
		/* MODELO -> VISTA 
		 * =============== */		 
		/********Trackers********/
		/**
		 * Modifica el estado del tracker de la vista.
		 * 
		 * @param name Nombre del tracker.
		 * @param status Estado del tracker.
		 */ 
		virtual void modificarStatusTracker(std::string name, 
		                                    std::string status);		
		
		/********Message********/		
		/**
		 * Muestra el mensaje pasado por parametro.
		 * 
		 * @param message Mensaje a mostrar.
		 */ 
		virtual void agregarMessage(std::string message);

		/********Refrescador********/		
		/**
		 * Actualiza la informacion de la pantalla.
		 */ 
		void actualizarPantalla();
		
		/**
		 * Comienza actualizar la pantalla.
		 */ 	
		virtual void comenzarARefrescarPantalla();		
};

/****************************************************************************/
#endif /*CONTROLADORGUI_H_*/
