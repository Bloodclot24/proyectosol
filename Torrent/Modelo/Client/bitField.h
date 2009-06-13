#ifndef BITFIELD_H_INCLUDED
#define BITFIELD_H_INCLUDED

/* Clase que representa un bitfield */
class BitField{
private:
     char* data; // el bitField
     int length; // la longitud en bits del bitfield

public:
     /* Crea un bitfield */
     BitField(int length);

     /* Copia los datos de *bitField */
     void setFields(char* bitField);

     /* cambia el estado de un bit del bitfield */
     void setField(int index, bool state);

     /* copia los datos de un bloque (8 bits) */
     void setBlock(char c, int blockIndex);

     /* obtiene el estado de un bit del bitfield */
     bool getField(int index) const;

     /* Obtiene la longitud del bitField en bits */
     int getLength() const;

     /* libera el area dedatos del bitfield */
     ~BitField();
};

#endif /* BITFIELD_H_INCLUDED */
