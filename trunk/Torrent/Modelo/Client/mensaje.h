#ifndef COMMON_MENSAJE_H_INCLUDED
#define COMMON_MENSAJE_H_INCLUDED

#include <stdint.h>
#include <iostream>
#include <stdio.h>
#include <string.h>

/** 
 * Clase que encapsula un mensaje. Se utiliza para enviar datos
 * atraves de un Socket.
 * 
 * @see Socket
 */
class Mensaje{
protected:
     unsigned int tamanio;	/**< El tamaño del buffer de datos */
     char* datos;		/**< El buffer de datos */
     friend class ThreadEmisor;

     /** 
      * Declara un constructor copia como privado para evitar
      * conflictos.
      * 
      * @param mensaje El Mensaje a copiar.
      */
     Mensaje(const Mensaje& mensaje){}

     /** 
      * Declara un constructor copia como privado para evitar
      * conflictos.
      * 
      * @param mensaje El Mensaje a copiar.
      */
     Mensaje operator=(const Mensaje& mensaje){ return mensaje;}

     
public:
     /** 
      * Crea un nuevo mensaje vacio
      */
     Mensaje(void);

     /** 
      * Devuelve el tamaño del buffer de datos.
      * @return El tamaño del buffer.
      */
     unsigned int getTamanio(void);
     
     /** 
      * Devuelve el buffer de datos del mensaje.
      * 
      * @return El buffer de datos.
      */
     char* getDatos(void);

     /** 
      * Copia datos al buffer de datos.
      * 
      * @param datos El buffer a copiar.
      * @param tamanio La longitud del buffer.
      */
     void copiarDatos(const char* datos, unsigned int tamanio);

     /** 
      * Asigna el buffer de datos. Este buffer es liberado cuando se
      * destruye el mensaje o se cambian los datos.
      * 
      * @param datos El nuevo buffer de datos.
      * @param tamanio El tamaño del buffer.
      */
     void asignarDatos(char* datos, unsigned int tamanio);
     
     /** 
      * Destruye el mensaje y libera el buffer de datos.
      */
     ~Mensaje();
};


#endif //COMMON_MENSAJE_H_INCLUDED
