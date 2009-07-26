#ifndef BITFIELD_H_INCLUDED
#define BITFIELD_H_INCLUDED

#include <stdint.h>
#include <stdio.h>
#include <string.h>

/** 
 * Clase que representa un bitfield.
 */
class BitField{
private:
     char* data;		/**< Los datos del bitfield */
     uint32_t length;		/**< Longitud en bits */
     uint32_t bytesLength;	/**< Longitud en bytes */

public:
     /** 
      * Crea un bitfield.
      * 
      * @param length La longitud en BITS del BitField a crear.
      */
     BitField(uint32_t length);

     /** 
      * Copia unbitfield;
      * 
      * @param original Elbitfield a copiar.
      */
     BitField(const BitField& original);

     /** 
      * Copia el contenido de un buffer y lo asigna como contenido del
      * BitField.
      * 
      * @param bitField El buffer desde donde se copian los datos.
      */
     void setFields(char* bitField);

     /** 
      * Cambia el estado de un bit del bitfield.
      * 
      * @param index El bit a cambiar.
      * @param state El estado a setear (true,false; 1,0)
      */
     void setField(uint32_t index, bool state);

     /** 
      * Copia los datos de un bloque (8 bits).
      * 
      * @param c Bloque de 8 bits de donde se copia el estado.
      *
      * @param blockIndex El numero de bloque a modificar.
      */
     void setBlock(char c, uint32_t blockIndex);

     /** 
      * Cambia el buffer de datos del BitField. Libera el buffer que
      * se utilizaba anteriormente para almacenar los datos y se
      * utiliza el provisto. El mismo se libera en la proxima llamada
      * a esta funcion o en el destructor.
      * 
      * @param data El buffer de datos.
      */
     void setData(char* data);

     /** 
      * Obtiene el estado de un bit del bitfield.
      * 
      * @param index El numero de bit.
      * 
      * @return true o false segun el estado del bit.
      */
     bool getField(uint32_t index) const;

     /** 
      * Obtiene la longitud del bitField en bits.
      * 
      * @return La longitud en bits del bitfield.
      */
     uint32_t getLength() const;
     
     /** 
      * Obtiene la longitud del bitField en bytes.
      * 
      * @return La longitud del BitField en bytes.
      */
     uint32_t getBytesLength() const;

     /** 
      * Obtiene el buffer de datos del BitField. Este buffer NO debe
      * ser liberado externamente.
      * 
      * @return El buffer de datos.
      */
     const char* getData() const;
	 
     /** 
      * Libera el area de datos del bitfield.
      */
     ~BitField();
};

#endif /* BITFIELD_H_INCLUDED */
