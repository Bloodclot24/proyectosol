#include <gtkmm.h>
#include "Controlador/controladorShell.h"
#include "Controlador/controladorVista.h"

int main(int argc, char *argv[]) {

   	if(argc > 1 && strcmp(argv[1], "consola") == 0) {
   		ControladorShell controlador;   
   		controlador.correr();	
   	} else {
		Gtk::Main kit(argc, argv);
   		ControladorVista controlador;   
   		controlador.correr();
	}
	
	return 0;
}
