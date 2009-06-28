#include "refreshVista.h"

/****************************************************************************/
RefreshVista::RefreshVista(Controlador* controlador): cVariable(&mutex) {
	
	this->corriendo= false;
	this->controlador= controlador;
}

/*--------------------------------------------------------------------------*/
void RefreshVista::run() {
	
	this->corriendo= true;
	
	while(corriendo) {
		mutex.lock();
		std::cout << "ESPERANDO!!!!!!!!!!!!!!!" << std::endl; 
		cVariable.wait();
		if(corriendo) {
			controlador->mostrarFiles();
			controlador->mostrarTrackers();
		}
		mutex.unlock();
	}
	
}

/*--------------------------------------------------------------------------*/
void RefreshVista::refrescar() {

	mutex.lock();
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

/****************************************************************************/
