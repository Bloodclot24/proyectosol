#include "sha1.h"

Sha1::Sha1()
{
	
}

std::string Sha1::intAstring(uint32_t valor)
{
	//convierto de int a string
	std::string snumero;
	std::stringstream cvz;
	cvz.width();
	cvz << valor;
	snumero = cvz.str();
	return snumero;
}


static uint32_t leftRotate(uint32_t numero, int cuanto){
     
     return  (numero << cuanto) | (numero >> 32-cuanto);
}

std::string Sha1::ejecutarSha1(std::string cadena)
{
	//escribo las constantes en BIG ENDIAN
     uint32_t h0 = 0x67452301;
     uint32_t h1 = 0xEFCDAB89;
     uint32_t h2 = 0x98BADCFE;
     uint32_t h3 = 0x10325476;
     uint32_t h4 = 0xC3D2E1F0;

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
	for(i=longitudCadena;(i % 448) != 0;i++){
		this->bitStream.appendBit(0);
	}
	//agrego los 64 bits 
	//del tamaño en bit's del mensaje original
	this->bitStream.append64BitsBE(tamanioMsjOriginal);
	//this->bitStream.append64BitsLE(tamanioMsjOriginal);
	
//	std::string bitString = this->bitStream.getBitString();

	longitudCadena = this->bitStream.length();

	//TODO
	//Divido el mensaje en partes de 512 bit
	std::string chunk;
	for(int parte=0;parte<(longitudCadena/64);parte++){
	     chunk.clear();
	     // obtengo la (parte)-esima parte
	     chunk.append(bitStream.getChunk(parte, 64));
	     
	     //Armo las palabras de 32 bits
	     uint32_t palabra[80];

	     //copio como Big Endian cada una de las palabras
	     for(int numPalabra=0;numPalabra<16;numPalabra++){
		  char* puntero = (char*)&palabra[numPalabra];
		  puntero[0] = chunk[numPalabra*4+3];
		  puntero[1] = chunk[numPalabra*4+2];
		  puntero[2] = chunk[numPalabra*4+1];
		  puntero[3] = chunk[numPalabra*4+0];
		  puntero++;
		  puntero--;
	     }

	     for(i=16;i<=79;i++){
		  palabra[i] = leftRotate(palabra[i-3] ^ palabra[i-8] ^ palabra[i-14] ^ palabra[i-16], 1);
	     }

	     for(i=0;i<80;i++){
		  std::cout.width(8);
		  std::cout.fill('0');
		  std::cout << std::hex << palabra[i] << std::endl;
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
		       k = 0x5A827999;
		  }
		  else{
		       if(i>=20 && i<=39){
			    f = b ^ c ^ d;
			    k = 0x6ED9EBA1;
		       }
		       if(i>=40 && i<=59){
			    f = (b & c) | (b & d) | (c & d);
			    k = 0x8F1BBCDC;
		       }
		       if(i>=60 && i<=79){
			    f = b ^ c ^ d;
			    k = 0xCA62C1D6;
		       }
		  }
		  auxiliar = leftRotate(a, 5) + f + e + k + palabra[i];
		  e = d;
		  d = c;
		  c = leftRotate(b, 30);
		  b = a;
		  a = auxiliar;
		  std::cout << "Paso " <<std::dec<< i <<": ";
		  std::cout.width(8);
		  std::cout.fill('0');
		  std::cout << std::hex << a << "  "<< b << "  " << c << "  " << d << "  " << e << std::endl;

	     }

	     h0 += a;
	     h1 += b;
	     h2 += c;
	     h3 += d;
	     h4 += e;

	     std::cout << "Parte : " << parte << std::endl;
	}
	//Resultado
	//Convierto h0...h4 de int a string y los concateno
// 	std::string resultadoH0 = intAstring(h0);
// 	std::string resultadoH1 = intAstring(h1);
// 	std::string resultadoH2 = intAstring(h2);
// 	std::string resultadoH3 = intAstring(h3);
// 	std::string resultadoH4 = intAstring(h4);
// 	std::string resultado = resultadoH0.append(resultadoH1.append(resultadoH2.append(resultadoH3.append(resultadoH4))));
	BitStream res;
	res.append32BitsBE(h0);
	res.append32BitsBE(h1);
	res.append32BitsBE(h2);
	res.append32BitsBE(h3);
	res.append32BitsBE(h4);
	return res.getString();
}

Sha1::~Sha1()
{
	
}
