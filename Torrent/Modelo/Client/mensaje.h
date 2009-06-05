#ifndef COMMON_MENSAJE_H_INCLUDED
#define COMMON_MENSAJE_H_INCLUDED

#include <stdint.h>
#include <iostream>

class Mensaje{
protected:
     unsigned int tamanio;
     char* datos;
     friend class ThreadReceptor;
     friend class ThreadEmisor;
     
public:
     /* Crea un nuevo mensaje vac�o */
     Mensaje(void);

     /* copia un mensaje */
     Mensaje(const Mensaje* mensaje);

     /* copia un mensaje */
     Mensaje(const Mensaje& mensaje);

     /* devuelve el tamamio de la seccion de datos */
     unsigned int getTamanio(void);
     
     /* devuelve la seccion de datos del mensaje */
     char* getDatos(void);

     /* copia datos a la seccion de datos */
     void copiarDatos(const char* datos, unsigned int tamanio);

     /* Asigna el buffer de datos. Este buffer es liberado cuando se
      * destruye el mensaje o se cambian los datos */
     void asignarDatos(char* datos, unsigned int tamanio);
     
     /* Destruye el mensaje y libera el area de datos */
     ~Mensaje();
};


#endif //COMMON_MENSAJE_H_INCLUDED