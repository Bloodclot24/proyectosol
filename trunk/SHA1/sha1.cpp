#include "sha1.h"

Sha1::Sha1()
{
	
}

std::string Sha1::ejecutarSha1(std::string cadena)
{
	//escribo las constantes en BIG ENDIAN
	uint32_t h0 = 0x01234567;//0x67452301
	uint32_t h1 = 0x89ABCDEF;//0xEFCDAB89
	uint32_t h2 = 0xFEDCBA98;//0x98BADCFE
	uint32_t h3 = 0x76543210;//0x10325476
	uint32_t h4 = 0xF0E1D2C3;//0xC3D2E1F0

	//TODO
	//pre-procesamiento
	uint64_t tamanioMsjOriginal = cadena.length();



	//TODO
	//Divido el mensaje en partes de 512 bits

		//Por cada parte.
	
		//Armo las palabras de 32 bits
		char* cadenaAux = new char[cadena.length()+1];
		uint32_t* palabra = NULL;
		int j = 0;
		int i = 0;
		for (i = 0; i< 512/32; i++,j+=8){
			palabra[i] = cadenaAux[j];
		}
		//TODO
		//convierto a Big Endian cada una de las palabras

	
		for(i=16;i<=79;i++){
			palabra[i] = (palabra[i-3] ^ palabra[i-8] ^ palabra[i-14] ^ palabra[i-16]) << 1;
		}
	
		uint32_t a = h0;
		uint32_t b = h1;
		uint32_t c = h2;
		uint32_t d = h3;
		uint32_t e = h4;

		for( i=0;i<=79;i++){
			if(i>=0 && i<=19){
				f = (b & c) | ((~b) & d);
				k = 0x9979825A;//0x5A827999
			}
			else{
				if(i>=20 && i<=39){
					f = b ^ c ^ d;
					k = 0xA1EBD96E;//0x6ED9EBA1
				}
				if(i>=40 && i<=59){
					f = (b & c) | (b & d) | (c & d);
					k = 0xDCBC1B8F;//0x8F1BBCDC
				}
				if(i>=60 && i<=79){
					f = b ^ c ^ d;
					k = 0xD6C162CA;//0xCA62C1D6
				}
			}
			temp = (a << 5) + f + e + k + palabra[i];
			e = d;
        	d = c;
        	c = b << 30;
        	b = a;
        	a = temp;

			h0 += a;
			h1 += b;
			h2 += c;
			h3 += d;
			h4 += e;
	 	}

	//TODO
	//Resultado
	std::string resultado = h0 append h1 append h2 append h3 append h4;
	return resultado;
}

Sha1::~Sha1()
{
	
}
