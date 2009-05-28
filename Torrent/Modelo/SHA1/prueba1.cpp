#include "bitStream.h"


int main(int argc,char** argv){
     BitStream stream;

     for(int i=0;i<32;i++){
	  stream.appendBit(1);
	  stream.appendBit(0);
     }

     stream.append64BitsLE(64);
     stream.append64BitsBE(64);
     
     
     std::cout << "Longitud final : " << stream.bitLength() << " bits\n";
     std::cout << "Cadena resultante: " << stream.getString() << "\n";
     std::cout << "Cadena de bits: " << stream.getBitString()  <<"\n";

}
