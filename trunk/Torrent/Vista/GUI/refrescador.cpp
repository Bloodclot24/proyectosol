#include "refrescador.h"

/****************************************************************************/
Refrescador::Refrescador(ControladorGUI* controlador) {
	
	this->controlador= controlador;
	this->corriendo= false;
}

/*--------------------------------------------------------------------------*/
void Refrescador::run() {

	this->corriendo= true;
	
	while(corriendo) {
		
		controlador->actualizarPantalla();	
		sleep(TIEMPO);		
	}
}

/*--------------------------------------------------------------------------*/
void Refrescador::comenzar() {
	
	start();
}

/*--------------------------------------------------------------------------*/
void Refrescador::terminar() {
	
	this->corriendo= false;
}

/****************************************************************************/

