#ifndef REFRESHVISTA_H_
#define REFRESHVISTA_H_

#include "../Modelo/Client/threads.h"
#include "../Controlador/controlador.h"

/****************************************************************************/
class RefreshVista: public Thread {
	
	private:
		Controlador* controlador;
		bool corriendo;
		Mutex mutex;
		CVariable cVariable;
	
	public:
		RefreshVista(Controlador* controlador);
		virtual void run();
		void stop();
		void refrescar();
};

/****************************************************************************/
#endif /*REFRESHVISTA_H_*/
class RefreshVista;
