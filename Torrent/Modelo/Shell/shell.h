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

#define MAX_FILE 45
#define MAX_PIECE 7
#define MAX_SIZE 10
#define MAX_DONE 7
#define MAX_STATUS 15
#define MAX_SPEED 15
#define MAX_NAME 20
#define MAX_SEED 7
#define MAX_CLIENT 20
#define MAX_IP 15
#define MAX_COMPLETED 12
#define MAX_BLOCKS 15
#define MAX_BLOCK 7
#define MAX_FILES 114
#define MAX_TRACKERS 42
#define MAX_PEERS 35
#define MAX_PIECES 51
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
#define SHOW_GENERAL "general"
#define SHOW_PEERS "peers"
#define SHOW_PIECES "pieces"

/*General Keys*/
#define HELP "help"
#define EXIT "exit"

/****************************************************************************/
class Shell {
		
	private:
		std::string command;
		Controlador* controlador;
		
	public:
		Shell(Controlador* controlador);
		
		void correr();	
		
		/*Archivo*/
		void mostrarArchivos();
		void mostrarArchivo(std::string file, int piece, std::string size,
		                    int done, std::string status, std::string 
		                    upSpeed, std::string downSpeed);
				                    
		/*General*/
		void mostrarGeneral(std::string file, std::string downloaded);
		
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
		void mostrarMensaje(std::string mensaje);
};

/****************************************************************************/
#endif /*SHELL_H_*/
class Shell;