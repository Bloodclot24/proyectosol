#ifndef SHA1_H_
#define SHA1_H_

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include "bitStream.h"

/****************************************************************************/
class Sha1 {

	private:
		BitStream bitStream;
        std::string intAstring(uint32_t valor);
        
	public:
		/**
		 * Realiza el algoritmo de SHA1, a la cadena pasada por parametro. 
		 * 
		 * @param cadena La cadena a codificar.
		 * 
		 * @return La cadena resultado.
		 */
		std::string ejecutarSha1(std::string cadena);
};

/****************************************************************************/
#endif /*SHA1_H_*/
