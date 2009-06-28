#include "bitField.h"
#include <math.h>
#include <iostream>

/* Crea un bitfield */
/****************************************************************************/
BitField::BitField(uint32_t length){
     this->length = length;
     bytesLength = ceil((double)length/8);
     data = new char[bytesLength];
     memset(data,0,bytesLength);
}

/* Copia los datos de *bitField */
/****************************************************************************/
void BitField::setFields(char* bitField){
     for(uint32_t i=0;i<bytesLength;i++)
	  data[i] = bitField[i];
}

/* copia los datos de un bloque (8 bits) */
/****************************************************************************/
void BitField::setBlock(char c, uint32_t blockIndex){
     if(blockIndex <bytesLength)
	  data[blockIndex] = c;
}

/* cambia la data del bitfield */
/****************************************************************************/
void BitField::setData(char* data) {	
	delete this->data;
	this->data= data;
}

/* cambia el estado de un bit del bitfield */
/****************************************************************************/
void BitField::setField(uint32_t index, bool state){
     if(index > length)
	  return;
     data[index/8] &= ~(1 << (7-(index%8)));
     if(state != 0)
	  data[index/8] |= (1 << (7-(index%8)));
}

/* Obtiene la longitud del bitField en bits */
/****************************************************************************/
uint32_t BitField::getLength() const{
     return length;
}
/* Obtiene la longitud del bitField en bytes */
/****************************************************************************/
uint32_t BitField::getBytesLength() const{
	return bytesLength;
}

/* obtiene el estado de un bit del bitfield */
/****************************************************************************/
bool BitField::getField(uint32_t index) const{
     if(index <= getLength())
	  return data[index/8] & (1 << (7-(index%8)));
     else{
	  std::cerr << "Se pidio el offset " << index << ", cuando el limite es " << getLength() << std::endl;
	  return 0;
     }
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
