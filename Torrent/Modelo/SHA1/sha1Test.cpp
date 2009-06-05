#include "sha1.h"
#include <iostream>

int main(int argc, char** argv){
     
    if(argc < 2)
	return -1;
     
     Sha1 hasher;

     std::string ejemplo(argv[1]);

     std::string hash = hasher.ejecutarSha1(ejemplo);

     for(int i=0;i<5;i++){
	  uint32_t *palabra = (uint32_t*)((hash.c_str())+i*4);
	  std::cout.width(4);
	  std::cout.fill('0');
	  std::cout << std::hex << *palabra; 
     }

     std::cout << std::endl;
}
