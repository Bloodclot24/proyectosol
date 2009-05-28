#include "sha1.h"

Sha1::Sha1()
{
	
}

std::string Sha1::intAstring(uint32_t valor)
{
	//convierto de int a string
	std::string snumero;
	std::stringstream cvz;
	cvz << valor;
	snumero = cvz.str();
	return snumero;
}

std::string Sha1::ejecutarSha1(std::string cadena)
{
	//escribo las constantes en BIG ENDIAN
	uint32_t h0 = 0x01234567;//0x67452301
	uint32_t h1 = 0x89ABCDEF;//0xEFCDAB89
	uint32_t h2 = 0xFEDCBA98;//0x98BADCFE
	uint32_t h3 = 0x76543210;//0x10325476
	uint32_t h4 = 0xF0E1D2C3;//0xC3D2E1F0

	//pre-procesamiento
	//obtengo el tamaño del mensaje original
	this->bitStream.setCadena(cadena);
	uint64_t tamanioMsjOriginal = this->bitStream.bitLength();//cadena.length();
	//agrego el bit 1 al final
	this->bitStream.appendBit(1);
	//obtengo la longitud  
	uint32_t longitudCadena = this->bitStream.bitLength();
	int i = 0;
	//completo con 0's hasta que la longitud en bits sea 448
	for(i=longitudCadena;/*i<=448*/(i % 448) != 0;i++){
		this->bitStream.appendBit(0);
	}
	//agrego los 64 bits 
	//del tamaño en bit's del mensaje original
	//this->bitStream.append64BitsBE(tamanioMsjOriginal);
	this->bitStream.append64BitsLE(tamanioMsjOriginal);
	
	std::string bitString = this->bitStream.getBitString();

	//TODO
	//Divido el mensaje en partes de 512 bits

		//Por cada parte.
	
		//Armo las palabras de 32 bits
		char* cadenaAux = new char[bitString.length()+1];
		uint32_t* palabra = NULL;
		int j = 0;
		for (i = 0; i< 512/32; i++,j+=8){
			palabra[i] = cadenaAux[j];
		}
		//TODO
		//convierto a Big Endian cada una de las palabras
		for(i = 0; i < 512/32; i++){
			palabra[i] = this->bitStream.swap32ABigEndian(palabra[i]);	
		}
	
		for(i=16;i<=79;i++){
			palabra[i] = (palabra[i-3] ^ palabra[i-8] ^ palabra[i-14] ^ palabra[i-16]) << 1;
		}
	
		uint32_t a = h0;
		uint32_t b = h1;
		uint32_t c = h2;
		uint32_t d = h3;
		uint32_t e = h4;
		uint32_t f = 0;
		uint32_t k = 0;
		uint32_t auxiliar = 0;

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
			auxiliar = (a << 5) + f + e + k + palabra[i];
			e = d;
        	d = c;
        	c = b << 30;
        	b = a;
        	a = auxiliar;

			h0 += a;
			h1 += b;
			h2 += c;
			h3 += d;
			h4 += e;
	 	}

	//Resultado
	//Convierto h0...h4 de int a string y los concateno
	std::string resultadoH0 = intAstring(h0);
	std::string resultadoH1 = intAstring(h1);
	std::string resultadoH2 = intAstring(h2);
	std::string resultadoH3 = intAstring(h3);
	std::string resultadoH4 = intAstring(h4);
	std::string resultado = resultadoH0.append(resultadoH1.append(resultadoH2.append(resultadoH3.append(resultadoH4))));
	return resultado;
}

Sha1::~Sha1()
{
	
}
