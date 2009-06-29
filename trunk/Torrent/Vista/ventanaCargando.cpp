#include "ventanaCargando.h"

/****************************************************************************/
VentanaCargando::VentanaCargando(ControladorGUI* controlador): cVariable(&mutex) {
	
	this->corriendo= false;
	this->pedido= false;
	this->controlador= controlador;
	this->showing= true;
}

/*--------------------------------------------------------------------------*/
void VentanaCargando::run() {
	
	this->corriendo= true;		

	while(corriendo) {

		mutex.lock();
		std::cout << "ESPERANDO Refresh!!" << std::endl; 
		if(!pedido) {
			cVariable.wait();
			pedido= false;
		}
		if(corriendo) {
			std::cout << "MOSTRANDO" << std::endl; 	
			if(showing)
				controlador->mostrarDialogDelay();
			showing= true;
		}
		mutex.unlock();
	}
}

/*--------------------------------------------------------------------------*/
void VentanaCargando::show() {

	mutex.lock();
	this->pedido= true;
	cVariable.signal();
	mutex.unlock();	
}

/*--------------------------------------------------------------------------*/
void VentanaCargando::stop() {

	this->corriendo= false;
	mutex.lock();
	cVariable.signal();
	mutex.unlock();	
}

/****************************************************************************/
