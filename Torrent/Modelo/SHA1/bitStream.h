#ifndef BITSTREAM_H_INCLUDED
#define BITSTREAM_H_INCLUDED

#include <stdint.h>
#include <iostream>

/****************************************************************************/
class BitStream {
	
	private:
    	std::string cadena;
    	int byteActual;
     	unsigned char bitActual;

	public:
		/**
		 * Crea un BitStream. 
		 */
		BitStream();
		
		/**
		 * Agrega un bit al final de la cadena. 
		 * 
		 * @param bit El bit agregar.
		 */
        void appendBit(int bit);
            
        /**
		 * Agrega 64 bits al final de la cadena tomando el numero
		 * como si estuviera en formato little endian. 
		 * 
		 * @param numero El numero agregar.
		 */
        void append64BitsLE(uint64_t numero);
     	
     	/**
		 * Agrega 64 bits al final de la cadena tomando el numero
		 * como si estuviera en formato big endian. 
		 * 
		 * @param numero El numero agregar.
		 */
     	void append64BitsBE(uint64_t numero);
     	
     	/**
		 * Agrega 32 bits al final de la cadena tomando el numero
		 * como si estuviera en formato big endian. 
		 * 
		 * @param numero El numero agregar.
		 */
     	void append32BitsBE(uint32_t numero);
	  	
	  	/**
		 * Agrega 32 bits al final de la cadena tomando el numero
		 * como si estuviera en formato little endian. 
		 * 
		 * @param numero El numero agregar.
		 */
	  	void append32BitsLE(uint32_t numero);
		
		/**
		 * Devuelve la longitud de la cadena en bits del bitStream. 
		 * 
		 * @return La longitud en bits.
		 */
		int bitLength();
		
		/**
		 * Devuelve la longitud de la cadena del bitStream. 
		 * 
		 * @return La longitud.
		 */
		int length();
	    
	    /**
		 * Devuelve la cadena del bitStream. 
		 * 
		 * @return La cadena.
		 */
	    const std::string& getString();
     	
     	/**
		 * Devuelve un fragmento de la cadena del bitStream. 
		 * 
		 * @param byteOffset Byte a partir del cual se comenzara a contar
		 * los bytes.
		 * @param bytes Cantidad de bytes requeridos.
		 * 
		 * @return El fragmento.
		 */
     	std::string getChunk(int byteOffset, int bytes);
	    
	    /**
		 * Devuelve la cadena del BitStream en bytes, en donde cada byte
		 * esta representado por bits. 
		 * 
		 * @return La cadena resultado.
		 */
	    const std::string getBitString();
		
		/**
		 * Modifica la cadena del BitStream. 
		 * 
		 * @param cadena La nueva cadena a ingresar.
		 */
		void setCadena(std::string cadena);
		
		/**
		 * Realiza el intercambio del numero pasado por paramatro
		 * a 32 bits big endian. 
		 * 
		 * @param numero El numero a realizar el intercambio.
		 * 
		 * @return El numero resultado.
		 */
		uint32_t swap32ABigEndian(uint32_t numero);
};

/****************************************************************************/
#endif //BITSTREAM_H_INCLUDED
