#ifndef SHELL_H_
#define SHELL_H_

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "../../Controlador/controlador.h"

#define PROMPT "SUN:~$ "
#define PARAMETRO " "
#define TITLE '-'
#define RELLENO ' '

#define MAX_AMOUNT 3
#define MAX_FILE 30
#define MAX_PIECE 7
#define MAX_SIZE 10
#define MAX_DONE 10
#define MAX_STATUS 15
#define MAX_SPEED 15
#define MAX_NAME 50
#define MAX_SEED 7
#define MAX_CLIENT 20
#define MAX_IP 15
#define MAX_COMPLETED 12
#define MAX_BLOCKS 15
#define MAX_BLOCK 7
#define MAX_FILES 113
#define MAX_TRACKERS 72
#define MAX_PEERS 35
#define MAX_TIME 15
#define MAX_KEY 51
#define MAX_DIC 51

/*Main Key*/
#define SHOW_TRACKERS "trackers"
#define SHOW_FILES "files"

/*Torrent Keys*/
#define ADD_TORRENT "torrent"
#define ADD_URL_TORRENT "url torrent"

/*File Keys*/
#define REMOVE "remove"
#define START "start"
#define PAUSE "pause"
#define STOP "stop"
#define SHOW_PEERS "peers"

/*General Keys*/
#define CLEAR "clear"
#define HELP "help"
#define EXIT "exit"

/****************************************************************************/
/**
 * Un intérprete de órdenes, intérprete de línea de órdenes, intérprete de 
 * comandos, terminal, consola, shell o su acrónimo en inglés CLI (por 
 * Command Line Interface) es un programa informático que actúa como interfaz
 * de usuario para comunicar al usuario con el sistema operativo mediante una 
 * ventana que espera órdenes escritas por el usuario en el teclado, los 
 * interpreta y los entrega al sistema operativo para su ejecución. La 
 * respuesta del sistema operativo se muestra al usuario en la misma ventana. 
 * A continuación, el programa shell queda esperando más instrucciones. Se 
 * interactúa con la información de la manera más sencilla posible, sin 
 * gráficas, sólo el texto crudo.
 */
class Shell {
		
	private:
		std::string command;
		Controlador* controlador;
		
	public:
		/**
		 * Crea un Shell. 
		 * 
		 * @param controlador Controlador del modelo.
		 */
		Shell(Controlador* controlador);
		
		/**
		 * Comienza a correr el shell. 
		 */
		void correr();	
		
		/*Archivo*/
		/**
		 * Muestra el encabezado de los datos a mostrar de los
		 * archivos. 
		 */
		void mostrarArchivos();
	
		/**
		 * Muestra el archivo pasado por parametro.
		 *
		 * @param amount Orden del archivo.
		 * @param file Nombre del archivo.
		 * @param size Tamanio del archivo.
		 * @param done Porcentaje del archivo completado.
		 * @param status Estado del archivo.
		 * @param downSpeed Velocidad de bajada.
		 * @param upSpeed Velocidad de subida.
		 * @param time Tiempo restante para completar la descarga.
		 */
		void mostrarArchivo(int amount, std::string file, std::string size,
		                    int done, std::string status, std::string upSpeed,
		                    std::string downSpeed, std::string ETA);
				                    
		/*General*/
		/**
		 * Muestra el estado general del archivo pasado por parametro.
		 *
		 * @param file Nombre del archivo.
		 * @param downloaded Porcentaje del archivo completado.
		 * @param information Informacion adicional del archivo.
		 */
		void mostrarGeneral(std::string file, std::string downloaded, 
							std::string information);
		
		/*Trackers*/
		/**
		 * Muestra el encabezado de los datos a mostrar de los
		 * trackers. 
		 */
		void mostrarTrackers();
		
		/**
		 * Muestra el tracker pasado por parametro.
		 *
		 * @param name Nombre del tracker.
		 * @param status Estado del tracker.
		 * @param seed Cantidad de seed del tracker.
		 */
		void mostrarTracker(std::string name, std::string status,
		                    int seed);

		/*Peers*/
		/**
		 * Muestra el encabezado de los datos a mostrar de los
		 * peers. 
		 */
		void mostrarPeers();
		
		/**
		 * Muestra el cliente pasado por parametro.
		 *
		 * @param name Nombre del tracker.
		 * @param status Estado del tracker.
		 * @param seed Cantidad de seed del tracker.
		 */
		void mostrarCliente(std::string ip, std::string estado);
		                                     
		/*Mensajes*/
		/**
		 * Muestra el mensaje pasado por parametro.
		 *
		 * @param mensaje Mensaje a mostrar.
		 */
		void mostrarMessage(std::string mensaje);
};

/****************************************************************************/
#endif /*SHELL_H_*/
class Shell;
