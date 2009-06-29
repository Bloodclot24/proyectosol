#ifndef CONTROLADOR_H_
#define CONTROLADOR_H_

#include <iostream>
#include <fstream>
#include <vector>

class Controlador;

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
/**
 * El controlador generico de la aplicacion Sun Torrent.
 */
class Controlador {
	
	protected:
		Client* cliente;
		std::vector<std::string> listaOrdenada; 
		
		bool validarExtensionFile(std::string path);
		std::string getEstadoTorrent(EstadoTorrent estado);
		Torrent* obtenerTorrent(std::string filename);
				
		/*Persistencia*/
		bool guardarConfig();
		bool cargarConfig();
		std::string crearCopiaTorrent(std::string pathTorrent);
		virtual void mostrarAnnounceUrlTorrent(Torrent* torrent)= 0;
						
	public:
		/**
		 * Crea un controlador. 
		 */
		Controlador();

		/**
		 * Destruye el controlador. 
		 */
		virtual ~Controlador();

		/**
		 * Comienza a correr la aplicacion, tanto el modelo como la vista. 
		 */
		virtual void correr()= 0;
		
		/* VISTA -> MODELO 
		 * =============== */
		/********Files********/
		/**
		 * Muestra todos los archivos en el panel de transferencias. 
		 */
		virtual void mostrarFiles()= 0;

		/**
		 * Agrega una descarga a partir de un archivo .torrent.
		 * 
		 * @param pathTorrent Directorio del archivo .torrent. 
		 */
		virtual bool addTorrent(std::string pathTorrent)= 0;

		/**
		 * Agrega una descarga a partir de una URL.
		 */
		virtual void addUrlTorrent()= 0;

		/**
		 * Elimina una descarga pasada por parametro.
		 * 
		 * @param file Nombre del archivo a eliminar. 
		 */
		virtual void removeFile(std::string file)= 0;

		/**
		 * Comienza una descarga pasada por parametro.
		 * 
		 * @param file Nombre del archivo a bajar. 
		 */
		virtual void startFile(std::string file)= 0;

		/**
		 * Pausa una descarga pasada por parametro.
		 * 
		 * @param file Nombre del archivo a pausar. 
		 */
		virtual void pauseFile(std::string file)= 0;
		
		/**
		 * Detiene una descarga pasada por parametro.
		 * 
		 * @param file Nombre del archivo a detener. 
		 */
		virtual void stopFile(std::string file)= 0;
		
		/********General********/
		/**
		 * Muestra el estado general del numero de archivo
		 * pasado por parametro. 
		 * 
		 * @param numfile Numero del archivo a mostrar. 
		 */
		virtual void mostrarGeneral(std::string numfile) {};
		
		/********Trackers********/
		/**
		 * Muestra todos los trackers. 
		 */
		virtual void mostrarTrackers()= 0;
		
		/********Peers********/
		/**
		 * Muestra los peers del numero de archivo
		 * pasado por parametro. 
		 * 
		 * @param numfile Numero del archivo a mostrar. 
		 */
		virtual void mostrarPeers(std::string numfile) {};
		
		/********Todas las Pestanias********/
		/**
		 * Muestra los peers del archivo pasado por parametro. 
		 * 
		 * @param filename Nombre del archivo que actualizara
		 * con sus detalles las pestanias. 
		 */
		virtual void actualizarPestanias(std::string filename) {};

		/********Orden********/
		/**
		 * Obtiene el orden del archivo pasado por parametro.
		 * 
		 * @param filename Nombre del archivo a buscar el orden.
		 */ 
		virtual uint32_t obtenerOrden(std::string filename) { return 0; };
		
		/********DialogDelay********/
		/**
		 * Muestra el dialogo de generacion de archivos.
		 */ 
		virtual void mostrarDialogDelay() {};

		/**
		 * Esconde el dialogo de generacion de archivos.
		 */ 
		virtual void esconderDialogDelay() {};
		
		/* MODELO -> VISTA 
		 * =============== */
		/********Files********/ 		 
		/**
		 * Modifica el estado del archivo pasado por parametro en el 
		 * en el panel de transferencias a DOWNLOADING.
		 * 
		 * @param filename Nombre del archivo.
		 */	
		virtual void start(std::string filename) {};
		
		/**
		 * Modifica el estado del archivo pasado por parametro en el 
		 * en el panel de transferencias a PAUSED.
		 * 
		 * @param filename Nombre del archivo.
		 */	
		virtual void pause(std::string filename) {};
		
		/**
		 * Modifica el estado del archivo pasado por parametro en el 
		 * en el panel de transferencias a STOPPED.
		 * 
		 * @param filename Nombre del archivo.
		 */	
		virtual void stop(std::string filename) {};
		
		/**
		 * Actualiza el porcentaje de completado del archivo pasado por
		 * parametro en el panel de transferencias.
		 * 
		 * @param filename Nombre del archivo.
		 * @param done Porcentaje del archivo completado.
		 */
		virtual void actualizarDone(std::string file, int done) {};
		
		/**
		 * Actualiza el estado del archivo pasado por parametro en el 
		 * panel de transferencias.
		 * 
		 * @param file Nombre del archivo.
		 * @param status Estado del archivo.
		 */		
		virtual void actualizarStatus(std::string file, std::string status) {};
		
		/**
		 * Actualiza velocidad de bajada del archivo pasado por parametro
		 * en el panel de transferencias.
		 * 
		 * @param file Nombre del archivo.
		 * @param downSpeed Velocidad de bajada.
		 */		
		virtual void actualizarDownSpeed(std::string file, std::string downSpeed) {};
		
		/**
		 * Actualiza velocidad de subida del archivo pasado por parametro
		 * en el panel de transferencias.
		 * 
		 * @param file Nombre del archivo.
		 * @param upSpeed Velocidad de subida.
		 */	
		virtual void actualizarUpSpeed(std::string file, std::string upSpeed) {};	
		
		/**
		 * Actualiza el tiempo restante del archivo pasado por parametro
		 * en el panel de transferencias.
		 * 
		 * @param file Nombre del archivo.
		 * @param time Tiempo restante para completar la descarga.
		 */	
		virtual void actualizarTime(std::string file, std::string time) {};	
		
		/********Trackers********/
		/**
		 * Modifica el estado del tracker de la vista.
		 * 
		 * @param name Nombre del tracker.
		 * @param status Estado del tracker.
		 */ 
		virtual void modificarStatusTracker(std::string name,
		                                    std::string status) {};		
		/**
		 * Agrega seed al tracker de la vista.
		 * 
		 * @param name Nombre del tracker.
		 * @param seed Nueva cantidad de seed.
		 */ 
		virtual void agregarSeedTracker(std::string name, int seed) {};

		/**
		 * Elimina el tracker pasado por parametro de la vista.
		 * 
		 * @param name Nombre del tracker.
		 */ 
		virtual void eliminarTracker(std::string name) {};
							
		/********Message********/		
		/**
		 * Muestra el mensaje pasado por parametro.
		 * 
		 * @param message Mensaje a mostrar.
		 */ 
		virtual void agregarMessage(std::string message)= 0;
};

/****************************************************************************/
#endif /*CONTROLADOR_H_*/
class Controlador;
