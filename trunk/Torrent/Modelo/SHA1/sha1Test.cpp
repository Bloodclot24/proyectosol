#include "sha1.h"
#include <iostream>

int main(int argc, char** argv){
     
     Sha1 hasher;

     std::string ejemplo("abc");

     std::string hash = hasher.ejecutarSha1(ejemplo);

     for(int i=0;i<hash.length();i++){
	  std::cout << hash[i]; 
     }

     std::cout << std::endl;
}
