#ifndef CONTROLADOR_H_
#define CONTROLADOR_H_

#include <iostream>
#include <fstream>
#include <vector>

class Controlador;

#include "../Modelo/Client/client.h"
#include "../Modelo/Client/torrent.h"
#include "../Modelo/Client/bitField.h"
#include "../Modelo/FileManager/fileManager.h"

#define EXTENSION ".torrent"
#define TAM_EXTENSION 8
#define PATH_DOWNLOADS  "Downloads"
#define PATH_CONFIG  "Downloads/Config/"
#define NAME_FILE_BF "Torrent"
#define EXTENSION_BITFIELD ".bf"
#define NAME_FILE_CONFIG "Downloads/Config/config.sun"
#define NAME_FILE_FALLO "Downloads/Config/fallo.sun"

/****************************************************************************/
/**
 * El controlador generico de la aplicacion Sun Torrent.
 */
class Controlador {
	
	protected:
		Client* cliente;
		std::vector<std::string> listaOrdenada; 
		
		bool validarExtensionFile(std::string path);
		Torrent* obtenerTorrent(std::string filename);
				
		/*Persistencia*/
		void guardarConfig();
		void cargarConfig();
		std::string crearCopiaTorrent(std::string pathTorrent);
		void crearAlertaFallo();
				
		virtual void mostrarAnnounceUrlTorrent(Torrent* torrent)= 0;

		/*Preparar Datos para mostrar en la vista*/
		std::string obtenerSize(uint64_t size);
		std::string obtenerDownloaded(double done);
		std::string obtenerVelocidad(uint32_t length);
		std::string obtenerETA(uint32_t time);		
		std::string obtenerStatus(EstadoTorrent status);
								
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
		
		/**
		 * Limpia las pestania peers y general. 
		 */
		virtual void limpiarPestanias() {};

		/********Orden********/
		/**
		 * Obtiene el orden del archivo pasado por parametro.
		 * 
		 * @param filename Nombre del archivo a buscar el orden.
		 */ 
		virtual uint32_t obtenerOrden(std::string filename)= 0;
			
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

		/********Refrescador********/	
		/**
		 * Comienza actualizar la pantalla.
		 */ 			
		virtual void comenzarARefrescarPantalla() {};	
};

/****************************************************************************/
#endif /*CONTROLADOR_H_*/
class Controlador;
