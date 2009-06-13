#include "bitField.h"

/* Crea un bitfield */
/****************************************************************************/
BitField::BitField(int length){
     this->length = length;
     data = new char[length/8];
     for(int i=0;i<length/8;i++)
	  data[i] = '\0';
}

/* Copia los datos de *bitField */
/****************************************************************************/
void BitField::setFields(char* bitField){
     for(int i=0;i<length/8;i++)
	  data[i] = bitField[i];
}

/* copia los datos de un bloque (8 bits) */
/****************************************************************************/
void BitField::setBlock(char c, int blockIndex){
     if(blockIndex < length/8)
	  data[blockIndex] = c;
}

/* cambia el estado de un bit del bitfield */
/****************************************************************************/
void BitField::setField(int index, int state){
     data[index/8] &= ~(1 << (7-(index&8)));
     if(state != 0)
	  data[index/8] |= (1 << (7-(index&8)));
}

/* Obtiene la longitud del bitField en bits */
/****************************************************************************/
int BitField::getLength(){
     return length;
}

/* obtiene el estado de un bit del bitfield */
/****************************************************************************/
int BitField::getField(int index){
     return data[index/8] & (1 << (7-(index&8)));
}

/* libera el area dedatos del bitfield */
/****************************************************************************/
BitField::~BitField(){
     delete data;
}
