#include "bitStream.h"

/****************************************************************************/
BitStream::BitStream() {
	
	byteActual=0;
	bitActual=0;
	cadena += '\0';
};

/*--------------------------------------------------------------------------*/
void BitStream::appendBit(int bit) {

	unsigned char bitNormalizado = bit>0?1<<(7-bitActual):0;

	if(bitActual <= 7){
		cadena[byteActual] = cadena[byteActual] & ~bitNormalizado;
	    cadena[byteActual] = cadena[byteActual] | bitNormalizado;
	    if(++bitActual >= 8){
			bitActual = 0;
		    byteActual++;
		    cadena += '\0';
		}
	}
	 else{
	      //No es posible la operacion de alta
	}
}

/*--------------------------------------------------------------------------*/
void BitStream::append64BitsLE(uint64_t numero) {
	 
	unsigned char bitActual=0;
	
	for(bitActual=0;bitActual<64;bitActual++) {
	 	uint64_t auxiliar=1;
	    auxiliar <<= bitActual;
	    appendBit(numero & auxiliar);
	}
}

/*--------------------------------------------------------------------------*/
void BitStream::append64BitsBE(uint64_t numero) {

	unsigned char bitActual=0;
	unsigned char byteActual=0;
	
	for(int i=7;i>=0;i--) {
		byteActual = ((char*)&numero)[i];
	    for(bitActual=0;bitActual<8;bitActual++)
			appendBit(byteActual & (1<<(7-bitActual)) );
	}
}

/*--------------------------------------------------------------------------*/
void BitStream::append32BitsBE(uint32_t numero) {
	  
	unsigned char bitActual=0;
	unsigned char byteActual=0;
	
	for(int i=3;i>=0;i--) {
		byteActual = ((char*)&numero)[3-i];
	    for(bitActual=0;bitActual<8;bitActual++)
			appendBit(byteActual & (1<<(7-bitActual)) );
	}
}

/*--------------------------------------------------------------------------*/
void BitStream::append32BitsLE(uint32_t numero) { 
	
	unsigned char bitActual=0;
	unsigned char byteActual=0;
	
	for(int i=3;i>=0;i--) {
		byteActual = ((char*)&numero)[i];
	    for(bitActual=0;bitActual<8;bitActual++)
			appendBit(byteActual & (1<<(7-bitActual)) );
	}
}

/*--------------------------------------------------------------------------*/
int BitStream::bitLength() {

	return byteActual*8+bitActual;
}

/*--------------------------------------------------------------------------*/
int BitStream::length() {

	return byteActual+(bitActual>0?1:0);
}

/*--------------------------------------------------------------------------*/
const std::string& BitStream::getString() {
	
	return cadena;
}

/*--------------------------------------------------------------------------*/
std::string BitStream::getChunk(int byteOffset, int bytes) {

	std::string chunk;
	
	for(int i=byteOffset;i<byteOffset+bytes;i++)
	       chunk += cadena[i];
	
	return chunk;
}

/*--------------------------------------------------------------------------*/
const std::string BitStream::getBitString() {
	  
	std::string cadenaFinal;
	int bitActual = 0;
	  
	for(int i=0;i<byteActual;i++) {
		for(bitActual=0;bitActual<8;bitActual++)
			cadenaFinal += (cadena[i]&(1<<bitActual))>0?'1':'0';
	}
	
	return cadenaFinal;
}

/*--------------------------------------------------------------------------*/
void BitStream::setCadena(std::string cadena) {

	this->cadena = cadena;
	byteActual = cadena.length();
	this->cadena += '\0';
}

/*--------------------------------------------------------------------------*/
uint32_t BitStream::swap32ABigEndian(uint32_t numero) {

	unsigned char* inicio = (unsigned char*)&numero;
	unsigned char aux;

    aux = inicio[3];
    inicio[3] = inicio[0];
    inicio[0] = aux;
    aux = inicio[2];
    inicio[2] = inicio[1];
    inicio[1] = aux;
    
    return numero;
}	

/****************************************************************************/
