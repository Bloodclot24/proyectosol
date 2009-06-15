#include <gtkmm.h>
#include "Controlador/ControladorShell/controladorShell.h"
#include "Controlador/ControladorGUI/controladorGUI.h"

int main(int argc, char *argv[]) {
	
   	if(argc > 1 && strcmp(argv[1], "console") == 0) {
   		ControladorShell* controlador = new ControladorShell();   
   		controlador->correr();	
   	} else {
		Gtk::Main kit(argc, argv);
   		ControladorGUI* controlador = new ControladorGUI();   
   		controlador->correr();
	}
	std::cout << "SALGO\n";
	
	return 0;
}
