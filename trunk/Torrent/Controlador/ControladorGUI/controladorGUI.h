#ifndef CONTROLADORVISTA_H_
#define CONTROLADORVISTA_H_

#include "../../Controlador/controlador.h"
#include "../../Vista/GUI/vistaTorrent.h"
class ControladorGUI;
#include "../../Vista/ventanaCargando.h"
/****************************************************************************/
/**
 * El controlador de la vista de la aplicacion Sun Torrent.
 */
class ControladorGUI: public Controlador {

	private:
		VistaTorrent* vista;
		VentanaCargando* ventanaCargando;
		
		int all;
		int downloading;
		int completed;
		int active;
		
		Mutex mutexActividades;
		void actualizarCantActividades();
		
		Mutex mutexMensaje;
		
		void mostrarFile(Torrent* torrent);
		
		Mutex mutexGeneral;
		void modificarGeneral(Torrent* torrent);
		
		Mutex mutexPeers;
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
			
		/********DialogDelay********/
		/**
		 * Muestra el dialogo de generacion de archivos.
		 */ 
		virtual void mostrarDialogDelay();
			
		/* MODELO -> VISTA 
		 * =============== */		 
		/********Files********/ 		 
		/**
		 * Modifica el estado del archivo pasado por parametro en el 
		 * en el panel de transferencias a DOWNLOADING.
		 * 
		 * @param file Nombre del archivo.
		 */	
		virtual void start(std::string filename);
		
		/**
		 * Modifica el estado del archivo pasado por parametro en el 
		 * en el panel de transferencias a PAUSED.
		 * 
		 * @param file Nombre del archivo.
		 */	
		virtual void pause(std::string filename);
		
		/**
		 * Modifica el estado del archivo pasado por parametro en el 
		 * en el panel de transferencias a STOPPED.
		 * 
		 * @param file Nombre del archivo.
		 */	
		virtual void stop(std::string filename);
		
		/**
		 * Modifica el estado del archivo pasado por parametro en el 
		 * en el panel de transferencias a COMPLETED.
		 * 
		 * @param file Nombre del archivo.
		 */	
		virtual void complete(std::string filename);
		
		/**
		 * Modifica el estado del archivo pasado por parametro en el 
		 * en el panel de transferencias a SEED.
		 * 
		 * @param file Nombre del archivo.
		 */	
		virtual void seed(std::string filename);
		
		/**
		 * Actualiza el porcentaje de completado del archivo pasado por
		 * parametro en el panel de transferencias.
		 * 
		 * @param file Nombre del archivo.
		 * @param done Porcentaje del archivo completado.
		 */
		virtual void actualizarDone(std::string file, int done);
		
		/**
		 * Actualiza el estado del archivo pasado por parametro en el 
		 * panel de transferencias.
		 * 
		 * @param file Nombre del archivo.
		 * @param status Estado del archivo.
		 */		
		virtual void actualizarStatus(std::string file, std::string status);
		
		/**
		 * Actualiza velocidad de bajada del archivo pasado por parametro
		 * en el panel de transferencias.
		 * 
		 * @param file Nombre del archivo.
		 * @param downSpeed Velocidad de bajada.
		 */		
		virtual void actualizarDownSpeed(std::string file, uint32_t downSpeed);
		
		/**
		 * Actualiza velocidad de subida del archivo pasado por parametro
		 * en el panel de transferencias.
		 * 
		 * @param file Nombre del archivo.
		 * @param upSpeed Velocidad de subida.
		 */	
		virtual void actualizarUpSpeed(std::string file, uint32_t upSpeed);	
		
		/**
		 * Actualiza el tiempo restante del archivo pasado por parametro
		 * en el panel de transferencias.
		 * 
		 * @param file Nombre del archivo.
		 * @param time Tiempo restante para completar la descarga.
		 */	
		virtual void actualizarTime(std::string file, int time);	
		
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
};

/****************************************************************************/
#endif /*CONTROLADORVISTA_H_*/
class ControladorVista;
