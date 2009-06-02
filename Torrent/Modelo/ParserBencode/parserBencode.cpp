#include "parserBencode.h"
#include <sstream>
#include <string>

/****************************************************************************/
std::list<BeNode*>* ParserBencode::beDecode(const char* NombreArchivo){
     
     std::fstream archTorrent;
     archTorrent.open(NombreArchivo, std::fstream::in);   
     std::stringbuf buf;

     archTorrent >> &buf;

     std::string str = buf.str();

     std::list<BeNode*> *list = new std::list<BeNode*>;

     BeNode* beNode;

     std::string::size_type endPosition=0;

     do{
	  beNode = beDecode(str, endPosition, endPosition);
	  list->push_back(beNode);
     }while(endPosition != BE_END);

     archTorrent.close();

     return list;

}

/****************************************************************************/
BeNode* ParserBencode::beDecode(std::string &cadena, 
				std::string::size_type inicioNodo,
				std::string::size_type &finNodo) {

     BeNode* beNode= new BeNode();
     std::string caracter;
     caracter.assign(cadena, inicioNodo, 1);
	
     if(caracter == START_INT) {
	  beNode->typeNode= BE_INT;
	  beNode->beInt= beDecodeInt(cadena, inicioNodo, finNodo);
	  
     } else if(caracter == START_LIST) {
	  beNode->typeNode = BE_LIST;
	  beNode->beList = beDecodeList(cadena, inicioNodo, finNodo); 
	  
     } else if(caracter == START_DICT) {
	  beNode->typeNode= BE_DICT;
	  beNode->beDict= beDecodeDict(cadena, inicioNodo, finNodo);
	  
     } else {
	  beNode->typeNode= BE_STR;
	  beNode->beStr= beDecodeStr(cadena, inicioNodo, finNodo);
     }
     
     if(finNodo+1 == cadena.length())
	  finNodo= BE_END;
     
     return(beNode);
}

/*--------------------------------------------------------------------------*/
std::string ParserBencode::beDecodeStr(std::string stringStr, 
				       std::string::size_type startPosition,
				       std::string::size_type &endPosition) {

     std::string::size_type startStr= stringStr.find(SEPARATOR, startPosition);
     std::string lenghtStr;
     lenghtStr.assign(stringStr, startPosition, startStr);
     stringStr.assign(stringStr, startStr+1, atoi(lenghtStr.c_str()));
     endPosition= startStr+atoi(lenghtStr.c_str());
     return(stringStr);			
}

/*--------------------------------------------------------------------------*/
long long ParserBencode::beDecodeInt(std::string stringInt, 
                                     std::string::size_type startPosition,
				     std::string::size_type &endPosition) {
	
     std::string::size_type startInt= stringInt.find(START_INT, startPosition);
     std::string::size_type endInt= stringInt.find(END, startInt+1);
     stringInt.assign(stringInt, startInt+1, endInt-startInt-1);
     long long Int= atoi(stringInt.c_str());
     endPosition= endInt;
     return(Int);	
}

/*--------------------------------------------------------------------------*/
BeList* ParserBencode::beDecodeList(std::string stringList, 
				    std::string::size_type startPosition,
				    std::string::size_type &endPosition) {
	
     BeList *beList = new BeList;

     BeNode* beNode;
     std::string caracter, next;
     caracter.assign(stringList, startPosition+1, 1);
	
     do {
	  beNode = beDecode(stringList, startPosition+1, endPosition);
	  beList->elements.push_back(beNode);
	  next.assign(stringList, endPosition+1, 1);
	  startPosition= endPosition;			
	  
     } while(next != END);
     endPosition++;
	
     return(beList);            	
}

/*--------------------------------------------------------------------------*/
BeDict* ParserBencode::beDecodeDict(std::string stringDict,
				    std::string::size_type startPosition,
				    std::string::size_type &endPosition) {
     BeDict *beDict = new BeDict;

     std::string next;
     next.assign(stringDict, startPosition+1, 1);
	
     while(next != END) {
	  std::string clave = beDecodeStr(stringDict, startPosition+1, endPosition);
	  BeNode* beNode = beDecode(stringDict, endPosition+1,
				    endPosition);
	  
	  startPosition= endPosition;
	  beDict->elements.insert(std::pair<std::string, BeNode*>(clave, beNode));
	  next.assign(stringDict, endPosition+1, 1);
     }
     endPosition++;
     return(beDict);
}

/****************************************************************************/

