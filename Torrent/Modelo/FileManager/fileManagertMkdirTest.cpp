#include "fileManager.h"

/****************************************************************************/
/*PRUEBA FILEMANAGER*/
/****************************************************************************/
int main(int argc, char *argv[]) {
	
	int creo= FileManager::crearDirectorio("chocolate"); 
	creo= FileManager::crearDirectorio("chocolate/amargo"); 
	
	FileManager fileManager("chocolate/amargo/pruebaMkdir",100);
	return 0;	
}

/****************************************************************************/
