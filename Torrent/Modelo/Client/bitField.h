#ifndef BITFIELD_H_INCLUDED
#define BITFIELD_H_INCLUDED

#include <stdint.h>

/* Clase que representa un bitfield */
class BitField{
private:
     char* data; // el bitField
     uint32_t length; // la longitud en bits del bitfield

public:
     /* Crea un bitfield */
     BitField(uint32_t length);

     /* Copia los datos de *bitField */
     void setFields(char* bitField);

     /* cambia el estado de un bit del bitfield */
     void setField(uint32_t index, bool state);

     /* copia los datos de un bloque (8 bits) */
     void setBlock(char c, uint32_t blockIndex);

     /* obtiene el estado de un bit del bitfield */
     bool getField(uint32_t index) const;

     /* Obtiene la longitud del bitField en bits */
     uint32_t getLength() const;
     
     /* Obtiene la longitud del bitField en bytes */
     uint32_t getBytesLength() const;

	 /* Obtiene los datos del bitField */
	 char* getData();
	 
     /* libera el area dedatos del bitfield */
     ~BitField();
};

#endif /* BITFIELD_H_INCLUDED */
