#ifndef BITSTREAM_H_INCLUDED
#define BITSTREAM_H_INCLUDED

#include <stdint.h>
#include <iostream>

class BitStream{
private:
     std::string cadena;
     int byteActual;
     unsigned char bitActual;

public:
     BitStream(){
	  byteActual=0;
	  bitActual=0;
	  cadena += '\0';
     };
     
     void appendBit(int bit){
	  unsigned char bitNormalizado = bit>0?1<<bitActual:0;

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
	       std::cout << "ERROR\n";
	  }
     }

     void append64BitsLE(uint64_t numero){
	  unsigned char bitActual=0;
	  for(bitActual=0;bitActual<64;bitActual++){
	       uint64_t auxiliar=1;
	       auxiliar <<= bitActual;
	       appendBit(numero & auxiliar );
	  }
     }

     void append64BitsBE(uint64_t numero){
	  unsigned char bitActual=0;
	  unsigned char byteActual=0;
	  for(int i=7;i>=0;i--){
	       byteActual = ((char*)&numero)[i];
	       for(bitActual=0;bitActual<8;bitActual++){
		    appendBit(byteActual & (1<<bitActual) );
	       }
	  }
     }

     int bitLength(void){
	  return byteActual*8+bitActual;
     }

     int length(void){
	  return byteActual+(bitActual>0?1:0);
     }

     const std::string& getString(void){
	  return cadena;
     }

     const std::string getBitString(void){
	  std::string cadenaFinal;
	  int bitActual = 0;
	 // unsigned char byte;
	  
	  for(int i=0;i<byteActual;i++){
	       for(bitActual=0;bitActual<8;bitActual++){
		    cadenaFinal += (cadena[i]&(1<<bitActual))>0?'1':'0';
	       }
	  }
	  return cadenaFinal;
     }

	void setCadena(std::string cadena){
		this->cadena = cadena;
		this->cadena += '\0';
	}

	uint32_t swap32ABigEndian(uint32_t numero){
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
};



#endif //BITSTREAM_H_INCLUDED
