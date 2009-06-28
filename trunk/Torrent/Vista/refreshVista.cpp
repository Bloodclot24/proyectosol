#include "refreshVista.h"

/****************************************************************************/
RefreshVista::RefreshVista(Controlador* controlador): cVariable(&mutex) {
	
	this->corriendo= false;
	this->pedido= false;
	this->controlador= controlador;
}

/*--------------------------------------------------------------------------*/
void RefreshVista::run() {
	
//	this->corriendo= true;
//	
//	while(corriendo) {
//		mutex.lock();
//		std::cout << "ESPERANDO Refresh!!" << std::endl; 
//		if(!pedido) {
//			cVariable.wait();
//			pedido= false;
//		}
//		
//		if(corriendo) {
//			controlador->mostrarFiles();
//			//controlador->mostrarTrackers();
//		}
//		std::cout << "YA PASO REFRESH Refresh!!" << std::endl;
//
//		sleep(10);
//
//		controlador->seCargo();
//
//
//		mutex.unlock();
//	}
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

/****************************************************************************/
