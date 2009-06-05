#include "parserBencode.h"

/*---------*/
void print(BeNode* beNode);

/*---------*/
void printStr(BeNode* beNode) {
	
	std::cout << beNode->beStr;
}

/*---------*/
void printInt(BeNode* beNode) {
	
	std::cout << beNode->beInt;
} 

/*---------*/
void printList(BeNode* beNode) {

	BeList* List = beNode->beList;

	std::cout << "List= [";	
    for(std::list<BeNode*>::iterator it= List->elements.begin(); 
		it != List->elements.end(); ++it) {	 
	  
		if(it != List->elements.begin())   
	  		std::cout << ", ";
	  		print(*it);
	} 
     
    std::cout << "]";
} 

/*---------*/
void printDict(BeNode* beNode) {
     
	BeDict* Dict = beNode->beDict;

    std::cout << "Dict= [";	
    for(std::map<std::string,BeNode*>::iterator it= Dict->elements.begin(); 
		it != Dict->elements.end(); ++it) {	 
	  
	if(it != Dict->elements.begin())   
		std::cout << ", ";
	  	std::cout << (*it).first << " => ";
	  	print((*it).second);
	} 
     
    std::cout << "]";
} 

/*---------*/
void print(BeNode* beNode){
	
	switch(beNode->typeNode) {
		case BE_STR:
			printStr(beNode);
		break;
		case BE_INT:
			printInt(beNode);
		break;
		case BE_LIST:
			printList(beNode);
		break;
		case BE_DICT:
			printDict(beNode);
		break;
		case BE_END:
			//Nada
		break;
	}
}

/*---------*/
int main(int argc, char *argv[]) {
   
   	ParserBencode parserBencode;

	std::cout << "PRUEBA TORRENT" << std::endl;
	std::cout << "==============" << std::endl;

	std::cout << 
	     "A continuacion se realizara la prueba de parseo de un archivo .torrent\
 completo. Presione [ENTER] para continuar. \n(NOTA: La informacion de \
las piezas del archivo puede resultar molesta y muy extensa. Asi es la vida)\n";

	char dummy[2];
	std::cin.getline(dummy,1);

	std::cout << "Torrent Sims"<< std::endl;
	std::cout << "------------"<< std::endl;
	
	std::list<BeNode*> *list= parserBencode.beDecode("prueba1.torrent");

	for(std::list<BeNode*>::iterator it= list->begin(); 
	    it != list->end(); ++it) {	 
	     print(*it);
	     std::cout << "\n";
	} 

	std::cout << "Torrent X Men Wolverine"<< std::endl;
	std::cout << "-----------------------"<< std::endl;
		
	list= parserBencode.beDecode("prueba2.torrent");

	for(std::list<BeNode*>::iterator it= list->begin(); 
	    it != list->end(); ++it) {	 
	     print(*it);
	     std::cout << "\n";
	} 
	
	return 0;
}