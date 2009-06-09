#include "Vista/vistaTorrent.h"
#include "Controlador/controlador.h"

int main(int argc, char *argv[]) {
   
   	//if(argc > 2 && strcmp(argv[1], "consola") == 0)
   	//	Controlador::getInstancia()->correrConsola();	
	//else {
		Gtk::Main kit(argc, argv);
		VistaTorrent vistaTorrent;
  		vistaTorrent.correr();
	//}
	
	return 0;
}
