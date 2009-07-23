#include "parserBencode.h"

/****************************************************************************/
std::list<BeNode*>* ParserBencode::beDecode(const char* nombreArchivo){
     std::fstream archTorrent;
     archTorrent.open(nombreArchivo, std::fstream::in);   
     std::stringbuf buf;
     
     archTorrent >> &buf;

     std::list<BeNode*> *list = beDecode(buf.str());
     
     archTorrent.close();

     return list;
}

/*--------------------------------------------------------------------------*/
std::list<BeNode*>* ParserBencode::beDecode(const std::string& buffer){
     std::list<BeNode*> *list= new std::list<BeNode*>;
    
     BeNode* beNode;

     std::string *str= new std::string(buffer);
     
     std::string::size_type endPosition= 0;

     do {
	  beNode= beDecode(str, endPosition, endPosition);
	  if(beNode != NULL)
	       list->push_back(beNode);
     } while(endPosition != BE_END && beNode != NULL);

     if(list->size() == 0){
	  delete list;
	  delete str;
	  list = NULL;
     }
	  
     return list;
}

/*--------------------------------------------------------------------------*/
BeNode* ParserBencode::beDecode(const std::string *cadena, 
				std::string::size_type inicioNodo,
				std::string::size_type &finNodo) {

     BeNode* beNode= new BeNode();
     std::string caracter;
     caracter.assign(*cadena, inicioNodo, 1);

     beNode->buffer = cadena;
     beNode->start = inicioNodo;
     
     if(caracter.compare(START_INT) == 0) {
	  beNode->typeNode= BE_INT;

	  beNode->beInt = beDecodeInt(cadena, inicioNodo, finNodo);

	  
     } else if(caracter.compare(START_LIST) == 0) {
	  beNode->typeNode= BE_LIST;
	  beNode->beList= beDecodeList(cadena, inicioNodo, finNodo); 
	  
     } else if(caracter.compare(START_DICT) == 0) {
	  beNode->typeNode= BE_DICT;
	  beNode->beDict= beDecodeDict(cadena, inicioNodo, finNodo);
	  
     } else{
	  beNode->typeNode= BE_STR;
	  beNode->beStr= beDecodeStr(cadena, inicioNodo, finNodo);

	  if(beNode->beStr.length() == 0){
	       delete beNode;
	       beNode=NULL;
	  }
     }

     if(beNode != NULL)
	  beNode->end   = finNodo; 

     if(finNodo+1 == cadena->length())
	  finNodo= BE_END;
     
     return(beNode);
}

/*--------------------------------------------------------------------------*/
std::string ParserBencode::beDecodeStr(const std::string *stringStr, 
				       std::string::size_type startPosition,
				       std::string::size_type &endPosition) {

     std::string auxiliar;
     std::string::size_type startStr= stringStr->find(SEPARATOR, 
						      startPosition);

     if(startStr != std::string::npos){
	  bool ok=true;
	  for(std::string::size_type i=startPosition;i<(startStr)&&ok; i++){
	       if(!isdigit((const int)(*stringStr)[i]))
		    ok=false;

	  }
	  if(ok){
	       std::string lenghtStr;
	       lenghtStr.clear();
	       lenghtStr.assign(*stringStr, startPosition, startStr-startPosition);
	       
	       auxiliar.assign(*stringStr, startStr+1, atoi(lenghtStr.c_str()));
	       if(atoi(lenghtStr.c_str()) == 0)
	       		auxiliar+= '\0';
	       
	       endPosition= startStr+atoi(lenghtStr.c_str());       
	  }
     }
     return(auxiliar);			
}

/*--------------------------------------------------------------------------*/
uint64_t ParserBencode::beDecodeInt(const std::string *stringInt, 
				    std::string::size_type startPosition,
				    std::string::size_type &endPosition) {
     
     std::string auxiliar;
     std::string::size_type startInt= stringInt->find(START_INT, 
						      startPosition);
     std::string::size_type endInt= stringInt->find(END, startInt+1);
     auxiliar.assign(*stringInt, startInt+1, endInt-startInt-1);
     uint64_t Int= atoll(auxiliar.c_str());
     endPosition= endInt;
     return(Int);	
}

/*--------------------------------------------------------------------------*/
BeList* ParserBencode::beDecodeList(const std::string *stringList, 
				    std::string::size_type startPosition,
				    std::string::size_type &endPosition) {
	
     BeList *beList= new BeList;

     BeNode* beNode;
     std::string caracter, next;
     caracter.assign(*stringList, startPosition+1, 1);
	
     do {
	  beNode= beDecode(stringList, startPosition+1, endPosition);
	  if(beNode != NULL){
	       beList->elements.push_back(beNode);
	       next.assign(*stringList, endPosition+1, 1);
	       startPosition= endPosition;
	  }
	  else{
	       next = END;
	  }
     } while(next != END);
     endPosition++;
	
     return(beList);            	
}

/*--------------------------------------------------------------------------*/
BeDict* ParserBencode::beDecodeDict(const std::string *stringDict,
				    std::string::size_type startPosition,
				    std::string::size_type &endPosition) {
     
    BeDict *beDict= new BeDict;

    std::string next;
    next.assign(*stringDict, startPosition+1, 1);
	
    while(next != END) {
	 std::string clave= beDecodeStr(stringDict, startPosition+1, 
					endPosition);
	 BeNode* beNode= beDecode(stringDict, endPosition+1, endPosition);
	 if(beNode != NULL){
	      startPosition = endPosition;
	      beDict->elements.insert(std::pair<std::string, BeNode*>(clave,
								      beNode));
	      next.assign(*stringDict, endPosition+1, 1);
	 }
	 else{
	      next = END;
	 }
    }
    endPosition++;
    
    return(beDict);
}

/*--------------------------------------------------------------------------*/
void ParserBencode::beFree(std::list<BeNode*>* nodeList){
     std::list<BeNode*>::iterator it;

     if(nodeList == NULL)
	  return;

      if( nodeList->front()->buffer!=NULL)
 	  delete nodeList->front()->buffer;

     for(it=nodeList->begin();it!=nodeList->end();it++)
	  beFree(*it);
}

/*--------------------------------------------------------------------------*/
void ParserBencode::beFree(BeNode* beNode){
     if(beNode == NULL)
	  return;

     switch(beNode->typeNode) {
     case BE_STR:
	  break;
     case BE_INT:
	  break;
     case BE_LIST:
	  beFree(beNode->beList);
	  break;
     case BE_DICT:
	  beFree(beNode->beDict);
	  break;
     case BE_END:
	  //Nada
	  break;
     }
     delete beNode;
}

/*--------------------------------------------------------------------------*/
void ParserBencode::beFree(BeList* list){
     if(list == NULL)
	  return;
     
     std::list<BeNode*>::iterator it;
     
     for(it=list->elements.begin();it!=list->elements.end();it++)
	  beFree(*it);

     delete list;
}

/*--------------------------------------------------------------------------*/
void ParserBencode::beFree(BeDict* dict){
     if(dict == NULL)
	  return;
     
     std::map<std::string, BeNode*>::iterator it;
     
     for(it=dict->elements.begin();it!=dict->elements.end();it++)
	  beFree((*it).second);

     delete dict;
}

/****************************************************************************/
