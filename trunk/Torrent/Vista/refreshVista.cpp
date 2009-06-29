#include "refreshVista.h"

/****************************************************************************/
RefreshVista::RefreshVista(Controlador* controlador): cVariable(&mutex) {
	
	this->corriendo= false;
	this->pedido= false;
	this->controlador= controlador;
	this->showing= true;
	this->hid= false;
}

/*--------------------------------------------------------------------------*/
void RefreshVista::run() {
	
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
			if(showing){
				this->hid= true;
				controlador->mostrarDialogDelay();
			}
			hid= false;
			showing= true;
		}
		mutex.unlock();
	}
}

/*--------------------------------------------------------------------------*/
void RefreshVista::refrescar() {

	mutex.lock();
	this->pedido= true;
	cVariable.signal();
	mutex.unlock();	
}

/*--------------------------------------------------------------------------*/
void RefreshVista::stop() {

	this->corriendo= false;
	mutex.lock();
	cVariable.signal();
	mutex.unlock();	
}

/*--------------------------------------------------------------------------*/
bool RefreshVista::hide() {
	
	if(!hid) {
		mutex.lock();
		showing= false;
		mutex.unlock();
	}
	return hid;
}

/****************************************************************************/
