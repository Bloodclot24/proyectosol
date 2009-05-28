#ifndef SHA1_H_
#define SHA1_H_

#include "bitStream.h"
#include <iostream>
#include <sstream>

class Sha1
{
	private:
		BitStream bitStream;
		
		std::string intAstring(uint32_t valor);
	public:
		Sha1();
		std::string ejecutarSha1(std::string cadena);
		~Sha1();
};

#endif /*SHA1_H_*/
