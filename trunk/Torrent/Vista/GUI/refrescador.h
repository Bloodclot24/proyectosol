#ifndef REFRESCADOR_H_
#define REFRESCADOR_H_

class Refrescador;
#include "../../Controlador/ControladorGUI/controladorGUI.h"

/****************************************************************************/
class Refrescador: public Thread {
	
	private:
		ControladorGUI* controlador;
		bool corriendo;
				
		 /** 
	      * Metodo principal de la clase.
	      * 
	      * @see Thread
	      */
	     virtual void run();

	public: 
		 /** 
      	  * Crea el hilo encargado de actualizar la pantalla.
          * 
          * @param controlador Controlador de la interfaz grafica.
          */
		Refrescador(ControladorGUI* controlador);
		
		 /** 
		  * Comienza actualizarse la pantalla.
		  */
		 void comenzar();
		
		 /** 
		  * Finaliza el thread.
		  * 
		  * @see Thread
		  */
		 void terminar();	
};

/****************************************************************************/
#endif /*REFRESCADOR_H_*/
class Refrescador;