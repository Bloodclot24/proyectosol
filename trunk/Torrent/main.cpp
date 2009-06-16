#include <gtkmm.h>
#include "Controlador/ControladorShell/controladorShell.h"
#include "Controlador/ControladorGUI/controladorGUI.h"

int main(int argc, char *argv[]) {
	
	Controlador* controlador;
	
   	if(argc > 1 && strcmp(argv[1], "console") == 0) {
   		controlador= new ControladorShell();   
   		controlador->correr();
   	} else {
		Gtk::Main kit(argc, argv);
   		controlador= new ControladorGUI();   
   		controlador->correr();
	}

	delete controlador;
	return 0;
}

