#ifndef SHELL_H_
#define SHELL_H_

#include <iostream>
#include <string.h>

#define PROMPT "SUN:~$ "

/*Main Key*/
#define SHOW_TRACKERS "trackers"
#define SHOW_PEERS "peers"
#define SHOW_GENERAL "general"
#define SHOW_ALL "all"
#define SHOW_DOWNLOADING "downloading"
#define SHOW_COMPLETED "completed"
#define SHOW_ACTIVE "active"
#define SHOW_INACTIVE "inactive"

/*Torrent Key*/
#define ADD_TORRENT "t"
#define ADD_URL_TORRENT "ut"

/*File Key*/
#define REMOVE "remove"
#define START "start"
#define PAUSE "pause"
#define STOP "stop"
#define SHOW_PIECES "pieces"

/*General Key*/
#define HELP "?"
#define EXIT "exit"

/****************************************************************************/
class Shell {
	
	private:
		std::string command;
		
	public:
		void correr();	
	
		/*Archivo*/
		void mostrarArchivo(std::string file, int piece, std::string size,
		                    int done, std::string status, std::string 
		                    downSpeed, std::string upSpeed);
				                    
		/*General*/
		void mostrarGeneralDownloaded(std::string downloaded,
		 							  std::string availability);
		
		/*Trackers*/
		void mostrarTracker(std::string name, std::string status,
		                    int seed);

		/*Peers*/
		void mostrarCliente(std::string ip, std::string cliente);
		                    
		/*Pieces*/
		void mostrarPieces(int number, std::string size, int blocks, int block,
		                   int completed);
};

/****************************************************************************/
#endif /*SHELL_H_*/