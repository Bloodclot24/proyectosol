#ifndef VENTANACARGANDO_H_
#define VENTANACARGANDO_H_

#include "../Modelo/Client/threads.h"
class VentanaCargando;
#include "../Controlador/ControladorGUI/controladorGUI.h"

/****************************************************************************/
/**
  * Ventana que informa que el archivo esta siendo generado.
  */ 
class VentanaCargando: public Thread {
	
	private:
		ControladorGUI* controlador;
		bool corriendo;
		bool pedido;
		Mutex mutex;
		CVariable cVariable;
	
	public:
		/**
  		  * Crea una VentanaCargando.
  		  * 
  		  * @param controlador Controlador del interfaz GUI.
  		  */ 
		VentanaCargando(ControladorGUI* controlador);
		
		/** 
          * Metodo principal de la clase. Toda la logica se desarrolla aca
          * dentro.
          * 
          * @see Thread
          */
		virtual void run();

		/** 
          * Saca del wait al hilo.
          * 
          * @see Thread
          */		
		void show();
		
		/** 
          * Finaliza el thread.
          * 
          * @see Thread
          */
		void stop();
};

/****************************************************************************/
#endif /*VENTANACARGANDO_H_*/
class VentanaCargando;
