#include "torrentFile.h"

#include "../ParserBencode/parserBencode.h"
#include <iostream>

#define INFO_NAME     "name"
#define INFO_PIECELEN "piece length"
#define INFO_PIECES   "pieces"
#define INFO_PRIVATE  "private"
#define INFO_FILES    "files"
#define INFO_LENGTH   "length"
#define INFO_MD5      "md5sum"
#define INFO_PATH     "path"

TorrentFile::TorrentFile(BeNode* node){
     if(node->typeNode == BE_DICT){
	  /* la seccion info debe ser un diccionario */
	  std::map<std::string, BeNode*>* dict = &(node->elements);
	  BeNode* elemento;
	  /* Extraigo todos los elementos que necesito */
	  try{
	       elemento = dict->at(INFO_NAME);
	       this->name = elemento->beStr;
	  }
	  catch(std::out_of_range &ex){}
	  try{
	       elemento = dict->at(INFO_PIECELEN);
	       this->pieceLength = elemento->beInt;
	  }
	  catch(std::out_of_range &ex){
	       this->pieceLength = 0;
	  }
	  try{
	       elemento = dict->at(INFO_PIECES);
	       this->pieces = elemento->beStr;
	  }
	  catch(std::out_of_range &ex){}
	  try{
	       elemento = dict->at(INFO_PRIVATE);
	       this->isPrivate = elemento->beInt;
	  }
	  catch(std::out_of_range &ex){
	       this->isPrivate = 0;
	  }
	  try{
	       elemento = dict->at(INFO_FILES);
	       /* Si existe, hay mas de un archivo*/
	       
	  }
	  catch(std::out_of_range &ex){
	       /* Si no existe hay un solo archivo */
	       
	  }
     }
}
