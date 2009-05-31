#include "../ParserBencode/parserBencode.h"
#include "torrent.h"

#define DICT_TRACKER  "announce"
#define DICT_DATE     "creation date"
#define DICT_COMMENTS "comments"
#define DICT_CREATOR  "created by"
#define DICT_ENCODING "encoding"
#define DICT_INFO     "info"

Torrent::Torrent(std::list<BeNode*>* info){
     BeNode* primero = info->front();
     
     if(primero->typeNode == BE_DICT){
	  std::map<std::string, BeNode*>* dict = &(primero->elements);
	  BeNode* elemento;
	  /* Extraigo todos los elementos que necesito */
	  try{
	       elemento = dict->at(DICT_TRACKER);
	       this->announce = elemento->beStr;
	  }
	  catch(std::out_of_range &ex){}
	  try{
	       elemento = dict->at(DICT_DATE);
	       this->creationDate = elemento->beInt;
	  }
	  catch(std::out_of_range &ex){
	       this->creationDate = 0;
	  }
	  try{
	       elemento = dict->at(DICT_COMMENTS);
	       this->acomment = elemento->beStr;
	  }
	  catch(std::out_of_range &ex){}
	  try{
	       elemento = dict->at(DICT_CREATOR);
	       this->createdBy = elemento->beStr;
	  }
	  catch(std::out_of_range &ex){}
	  try{
	       elemento = dict->at(DICT_ENCODING);
	       this->encoding = elemento->beInt;
	  }
	  catch(std::out_of_range &ex){
	       this->encoding = 0;
	  }
	  try{
	       /* Informacion de todos los archivos */
	       elemento = dict->at(DICT_INFO);
	       /* TODO */
	  }
	  catch(std::out_of_range &ex){}
     }
}
