#include "common_mensaje.h"

/* Crea un nuevo mensaje vacío */
Mensaje::Mensaje(void){
     tamanio = 0;
     datos = NULL;
}

/* Copia un mensaje */
Mensaje::Mensaje(const Mensaje* mensaje){
     tamanio = mensaje->tamanio;
     if(tamanio > 0 && mensaje->datos != NULL){
	  datos = new char[tamanio];
	  for(int i=0;i<tamanio;i++)
	       datos[i]=mensaje->datos[i];
     }
}

/* Copia un mensaje */
Mensaje::Mensaje(const Mensaje& mensaje){
     tamanio = mensaje.tamanio;
     if(tamanio > 0 && mensaje.datos != NULL){
	  datos = new char[tamanio];
	  for(int i=0;i<tamanio;i++)
	       datos[i]=mensaje.datos[i];
     }
}

/* devuelve el tamamio de la seccion de datos */
unsigned int Mensaje::getTamanio(void){
     return tamanio;
}
     
/* devuelve la seccion de datos del mensaje */
char* Mensaje::getDatos(void){
     return datos;
}

/* copia datos a la seccion de datos */
void Mensaje::copiarDatos(const char* datos, unsigned int tamanio){
     if(this->datos != NULL)
	  delete[] this->datos;
     this->tamanio = tamanio;
     this->datos = new char[tamanio];
     for(int i=0;i<tamanio;i++)
	  this->datos[i] = datos[i];
}

/* Asigna el buffer de datos. Este buffer es liberado cuando se
 * destruye el mensaje o se cambian los datos */
void Mensaje::asignarDatos(char* datos, unsigned tamanio){
     if(this->datos != NULL)
	  delete[] this->datos;
     this->tamanio = tamanio;
     this->datos = datos;
}
     
/* Destruye el mensaje y libera los datos */
Mensaje::~Mensaje(){
     if(datos != NULL)
	  delete[] datos;
}
