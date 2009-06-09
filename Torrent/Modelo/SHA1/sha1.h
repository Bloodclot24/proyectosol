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
     std::string ejecutarSha1(std::string cadena);
};

#endif /*SHA1_H_*/
