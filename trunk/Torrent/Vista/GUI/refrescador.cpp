#include "refrescador.h"

/****************************************************************************/
Refrescador::Refrescador(ControladorGUI* controlador): cVariable(&mutex) {
	
	this->controlador= controlador;
	this->corriendo= false;
}

/*--------------------------------------------------------------------------*/
void Refrescador::run() {

	this->corriendo= true;
	
	while(corriendo) {
		
		//mutex.lock();
				
		std::cout << "******************Actualizandooooo******************" << std::endl;
		controlador->actualizarPantalla();	

		//mutex.unlock();
		
		sleep(10);		
	}

	this->corriendo= false;		
}

/*--------------------------------------------------------------------------*/
void Refrescador::comenzar() {
	
	start();
}

/*--------------------------------------------------------------------------*/
void Refrescador::finish() {
	
	//mutex.lock();
	this->corriendo= false;
	//mutex.unlock();
}

/****************************************************************************/
