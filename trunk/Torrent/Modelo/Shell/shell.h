#ifndef SHELL_H_
#define SHELL_H_

#include <iostream>
#include <string.h>

#define PROMPT "SUN:~$ "
#define ESPACIO "     "

/*Main Key*/
#define SHOW_TRACKERS "trackers"
#define SHOW_GENERAL "general"
#define SHOW_ALL "all"
#define SHOW_DOWNLOADING "downloading"
#define SHOW_COMPLETED "completed"
#define SHOW_ACTIVE "active"
#define SHOW_INACTIVE "inactive"

/*Tracker Keys*/
#define SHOW_PEERS "peers"

/*Torrent Keys*/
#define ADD_TORRENT "torrent"
#define ADD_URL_TORRENT "url torrent"

/*File Keys*/
#define REMOVE "remove"
#define START "start"
#define PAUSE "pause"
#define STOP "stop"
#define SHOW_PIECES "pieces"

/*General Keys*/
#define HELP "help"
#define EXIT "exit"
#define PARAMETRO " "

/****************************************************************************/
class Shell {
		
	private:
		std::string command;
		
		void mostrarHelp();
		
	public:
		void correr();	
		
		/*Archivo*/
		void mostrarArchivos();
		void mostrarArchivo(std::string file, int piece, std::string size,
		                    int done, std::string status, std::string 
		                    upSpeed, std::string downSpeed);
				                    
		/*General*/
		void mostrarGeneralDownloaded(std::string downloaded,
		 							  std::string availability);
		
		/*Trackers*/
		void mostrarTrackers();
		void mostrarTracker(std::string name, std::string status,
		                    int seed);

		/*Peers*/
		void mostrarPeers();
		void mostrarCliente(std::string ip, std::string cliente);
		                    
		/*Pieces*/
		void mostrarPieces();
		void mostrarPiece(int number, std::string size, int blocks, int block,
		                  int completed);
		                  
		/*Mensajes*/
		void archivoNoEncontrado();
};

/****************************************************************************/
#endif /*SHELL_H_*/
