#ifndef CONTROLADORSHELL_H_
#define CONTROLADORSHELL_H_

#include <stdlib.h>
#include "../../Controlador/controlador.h"
#include "../../Vista/Shell/shell.h"

/****************************************************************************/
/*
 * El controlador del shell de la aplicacion Sun Torrent.
 */
class ControladorShell: public Controlador {
	
	private:
		Shell* shell;
		
		bool validarNumFile(std::string numFile);
		uint32_t contadorOrden;
		int cantFiles;
		const char* obtenerFilename(std::string numFile);
	
	protected:
		virtual void mostrarAnnounceUrlTorrent(Torrent* torrent);
		
	public:
		/**
		 * Crea un controladorShell. 
		 */
		ControladorShell(); 
		
		/**
		 * Destruye el controladorShell. 
		 */
		virtual ~ControladorShell();
	
		/**
		 * Comienza a correr la aplicacion, tanto el modelo como el shell. 
		 */
		virtual void correr();

		/* VISTA -> MODELO 
		 * =============== */
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
		 * Muestra todos los trackers. 
		 */	
		virtual void mostrarTrackers();
			
		/**
		 * Muestra todos los archivos en el panel de transferencias. 
		 */
		virtual void mostrarFiles();
		
		/**
		 * Muestra el estado general del numero de archivo
		 * pasado por parametro. 
		 * 
		 * @param numfile Numero del archivo a mostrar. 
		 */
		virtual void mostrarGeneral(std::string numFile);	
		
		/**
		 * Muestra los peers del numero de archivo
		 * pasado por parametro. 
		 * 
		 * @param numfile Numero del archivo a mostrar. 
		 */
		virtual void mostrarPeers(std::string numFile);
		
		/**
		 * Elimina una descarga pasada por parametro.
		 * 
		 * @param file Nombre del archivo a eliminar. 
		 */
		virtual void removeFile(std::string numFile);
			
		/**
		 * Comienza una descarga pasada por parametro.
		 * 
		 * @param file Nombre del archivo a bajar. 
		 */
		virtual void startFile(std::string numFile);
		
		/**
		 * Pausa una descarga pasada por parametro.
		 * 
		 * @param file Nombre del archivo a pausar. 
		 */
		virtual void pauseFile(std::string numFile);
		
		/**
		 * Detiene una descarga pasada por parametro.
		 * 
		 * @param file Nombre del archivo a detener. 
		 */
		virtual void stopFile(std::string numFile);
		
		/**
		 * Obtiene el orden del archivo pasado por parametro.
		 * 
		 * @param filename Nombre del archivo a buscar el orden.
		 */ 
		virtual uint32_t obtenerOrden(std::string filename);
	
		/* MODELO -> VISTA 
		 * =============== */		 
		/********Message********/		
		/**
		 * Muestra el mensaje pasado por parametro.
		 * 
		 * @param message Mensaje a mostrar.
		 */ 
		virtual void agregarMessage(std::string message);
};

/****************************************************************************/
#endif /*CONTROLADORSHELL_H_*/
class ControladorShell;
