#ifndef REFRESCADOR_H_
#define REFRESCADOR_H_

class Refrescador;
#include "../../Controlador/ControladorGUI/controladorGUI.h"

/****************************************************************************/
class Refrescador: public Thread {
	
	private:
		ControladorGUI* controlador;
		
		int all;
		int downloading;
		int completed;
		int active;
		int inactive;
	
		 /** 
	      * Metodo principal de la clase. Toda la logica se desarrolla aca
	      * dentro.
	      * 
	      * @see Thread
	      */
	     virtual void run();
	     
	     bool corriendo;
	     Mutex mutex;
	     CVariable cVariable;
	
	public: 
		 /** 
      	  * Crea el hilo encargado de actualizar la pantalla.
          * 
          * @param controlador Controlador de la interfaz grafica.
          */
		Refrescador(ControladorGUI* controlador);
		
		 /** 
		  * Comienza el procesamiento y flujo de datos.
		  * 
		  * @param hash El hash que identifica al Torrent.
		  */
		 void comenzar();
		
		 /** 
		  * Finaliza el thread y libera memoria.
		  * 
		  * @see Thread
		  */
		 void finish();	
};

/****************************************************************************/
#endif /*REFRESCADOR_H_*/
class Refrescador;