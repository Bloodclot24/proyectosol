#include "parserBencode.h"

void print(BeNode* beNode);

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
   
   	std::fstream archTorrent;
   	archTorrent.open("prueba.torrent", std::fstream::in);   
   
	char linea[512];
	archTorrent.getline(linea, 10);  
   	ParserBencode parserBencode;
	std::string::size_type endPosition;
	
/*--------------------------------------------------------------------------*/
	/*PRUEBA STR*/
	std::cout << "PRUEBA STR" << std::endl;
	std::cout << "==========" << std::endl;
	
	std::string stringStr= "6:correo4:spam";
	std::cout << "stringStr: "<< stringStr << std::endl;
	
	BeNode* beNode= parserBencode.beDecode(stringStr, 0, endPosition);
	print(beNode);
	std::cout << "\n ";
		

	beNode= parserBencode.beDecode(stringStr, endPosition+1, endPosition);
	print(beNode);
	std::cout << "\n ";
		
/*--------------------------------------------------------------------------*/
	/*PRUEBA INT*/
	std::cout << "PRUEBA INT" << std::endl;
	std::cout << "==========" << std::endl;
	
	std::string stringInt= "i5000000ei13e";
	std::cout << "stringInt: "<< stringInt << std::endl;
	
	beNode= parserBencode.beDecode(stringInt, 0, endPosition);
	print(beNode);
	std::cout << "\n";
	
	beNode= parserBencode.beDecode(stringInt, endPosition+1, endPosition);
	print(beNode);	
	std::cout << "\n";
	
/*--------------------------------------------------------------------------*/
	/*PRUEBA LIST*/
	std::cout << "PRUEBA LIST1" << std::endl;
	std::cout << "============" << std::endl;
		
	std::string stringList= "l1:A1:B1:Cel2:pc8:notebooke";
	std::cout << "stringList1: "<< stringList << std::endl;
	
	beNode= parserBencode.beDecode(stringList, 0, endPosition);
	print(beNode);
	std::cout << "\n";

	beNode= parserBencode.beDecode(stringList, endPosition+1, endPosition);
	printList(beNode);
	std::cout << "\n";

	std::cout << "PRUEBA LIST2" << std::endl;
	std::cout << "============" << std::endl;
		
	stringList= "ll1:A1:Bel1:E1:Fel1:C1:Dee";
	std::cout << "stringList2: "<< stringList << std::endl;
	
	beNode= parserBencode.beDecode(stringList, 0, endPosition);
	print(beNode);
	std::cout << "\n";
	
/*--------------------------------------------------------------------------*/
	/*PRUEBA DICT*/
	std::cout << "PRUEBA DICT1" << std::endl;
	std::cout << "============" << std::endl;

	std::string stringDict= "d4:vaca3:muu4:gato4:miaue";
	std::cout << "stringDict1: "<< stringDict << std::endl;
	beNode= parserBencode.beDecode(stringDict, 0, endPosition);
	print(beNode);
	std::cout << "\n";

	std::cout << "PRUEBA DICT2" << std::endl;
	std::cout << "============" << std::endl;
	
	stringDict= "d5:perrol4:guau4:boufe4:pato4:cuaked4:vaca3:muu4:gato4:miaue";
	std::cout << "stringDict3: "<< stringDict << std::endl;
	beNode= parserBencode.beDecode(stringDict, 0, endPosition);
	print(beNode);
	std::cout << "\n";

	beNode= parserBencode.beDecode(stringDict, endPosition+1, endPosition);
	print(beNode);
	std::cout << "\n";

	std::cout << "PRUEBA DICT3" << std::endl;
	std::cout << "============" << std::endl;
	
	stringDict= "d5:perroll4:guau4:boufeee";
	std::cout << "stringDict3: "<< stringDict << std::endl;
	beNode= parserBencode.beDecode(stringDict, 0, endPosition);
	print(beNode);
	std::cout << "\n";

	beNode= parserBencode.beDecode(stringDict, endPosition+1, endPosition);
	print(beNode);
	std::cout << "\n";
	
//	std::cout << "PRUEBA DICT4" << std::endl;
//	std::cout << "============" << std::endl;
//	
//	stringDict= "dd5:perro4:guauee";
//	std::cout << "stringDict4: "<< stringDict << std::endl;
//	beNode= parserBencode.beDecode(stringDict, 0, endPosition);
//	printDict(beNode->beDict, endPosition);

/*--------------------------------------------------------------------------*/	
// 	/*PRUEBA 1*/
// 	std::cout << "PRUEBA 1" << std::endl;
// 	std::cout << "========" << std::endl;

// 	std::string stringPrueba1= "6:correod4:vaca3:muu4:gato4:miaue";	
// 	std::cout << "stringPrueba1: "<< stringPrueba1 << std::endl;
	
// 	endPosition = -1;
// 	print(stringPrueba1, endPosition, parserBencode);
	
/*--------------------------------------------------------------------------*/	
//	/*PRUEBA 2*/
//	std::cout << "PRUEBA 2" << std::endl;
//	std::cout << "========" << std::endl;
//
//	std::string stringPrueba2= "d8:announce40:http://tracker.thepiratebay.org/announce13:announce-listll40:http://tracker.thepiratebay.org/announceel42:udp://tracker.thepiratebay.org:80/announceee7:comment47:Torrent downloaded from http://thepiratebay.org13:creation datei1242619527e4:infod6:lengthi6011879424e4:name12:rld-sim3.iso12:piece lengthi4194304e6:pieceseee";
//	std::cout << "stringPrueba2: "<< stringPrueba2 << std::endl;
//	
//	endPosition= -1;
//	print(stringPrueba2, endPosition, parserBencode);


/*--------------------------------------------------------------------------*/	

	archTorrent.close();

	return 0;
}
