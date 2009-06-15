#include "bitField.h"
#include <math.h>

/* Crea un bitfield */
/****************************************************************************/
BitField::BitField(uint32_t length){
     this->length = length;
     uint32_t aux = ceil(length/8);
     data = new char[aux];
     for(int i=0;i<ceil(length/8);i++)
	  data[i] = '\0';
}

/* Copia los datos de *bitField */
/****************************************************************************/
void BitField::setFields(char* bitField){
     for(int i=0;i<ceil(length/8);i++)
	  data[i] = bitField[i];
}

/* copia los datos de un bloque (8 bits) */
/****************************************************************************/
void BitField::setBlock(char c, uint32_t blockIndex){
     if(blockIndex < ceil(length/8))
	  data[blockIndex] = c;
}

/* cambia el estado de un bit del bitfield */
/****************************************************************************/
void BitField::setField(uint32_t index, bool state){
     if(index > length)
	  return;
     data[index/8] &= ~(1 << (7-(index&8)));
     if(state != 0)
	  data[index/8] |= (1 << (7-(index&8)));
}

/* Obtiene la longitud del bitField en bits */
/****************************************************************************/
uint32_t BitField::getLength() const{
     return length;
}
/* Obtiene la longitud del bitField en bytes */
/****************************************************************************/
uint32_t BitField::getBytesLength() const{
	return ceil(length/8);
}

/* obtiene el estado de un bit del bitfield */
/****************************************************************************/
bool BitField::getField(uint32_t index) const{
     return data[index/8] & (1 << (7-(index&8)));
}

/****************************************************************************/
char* BitField::getData(){
	return data;
}	

/* libera el area dedatos del bitfield */
/****************************************************************************/
BitField::~BitField(){
     delete data;
}
