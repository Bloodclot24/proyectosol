#include "../ParserBencode/parserBencode.h"
#include "torrent.h"

#include <stdexcept>

#define DICT_TRACKER  "announce"
#define DICT_DATE     "creation date"
#define DICT_COMMENTS "comments"
#define DICT_CREATOR  "created by"
#define DICT_ENCODING "encoding"
#define DICT_INFO     "info"


Torrent::Torrent(std::list<BeNode*>* info){
     BeNode* primero = info->front();
     
     if(primero->typeNode == BE_DICT){
	  std::map<std::string, BeNode*>* dict = &(primero->beDict->elements);
	  BeNode* elemento;
	  /* Extraigo todos los elementos que necesito */
	  
	  elemento = (*dict)[DICT_TRACKER];
	  if(elemento != NULL)
	       this->announce = elemento->beStr;

	  elemento = (*dict)[DICT_DATE];
	  if(elemento != NULL)
	       this->creationDate = elemento->beInt;
	  else this->creationDate = 0;

	  elemento = (*dict)[DICT_COMMENTS];
	  if(elemento != NULL)
	       this->comment = elemento->beStr;

	  elemento = (*dict)[DICT_CREATOR];
	  if(elemento != NULL)
	       this->createdBy = elemento->beStr;

	  elemento = (*dict)[DICT_ENCODING];
	  if(elemento != NULL)
	       this->encoding = elemento->beInt;
	  else this->encoding = 0;

	  /* Informacion de todos los archivos */
	  elemento = (*dict)[DICT_INFO];
	  archivos = TorrentFile::Parse(elemento);
     }
}
