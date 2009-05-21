#include "Vista/vistaTorrent.h"

int main(int argc, char *argv[]) {
   
	Gtk::Main kit(argc, argv);

	VistaTorrent vistaTorrent;
  	
  	vistaTorrent.correr();

	return 0;
} 