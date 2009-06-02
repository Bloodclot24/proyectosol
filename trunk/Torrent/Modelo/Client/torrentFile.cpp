#include "torrentFile.h"

#include "../ParserBencode/parserBencode.h"
#include <iostream>
#include <stdexcept>

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
	  std::map<std::string, BeNode*>* dict = &(node->beDict->elements);
	  BeNode* elemento;
	  /* Extraigo todos los elementos que necesito */

	  elemento = (*dict)[INFO_NAME];
	  if(elemento != NULL)
	       this->name = elemento->beStr;
	  
	  elemento = (*dict)[INFO_PIECELEN];
	  if(elemento != NULL)
	       this->pieceLength = elemento->beInt;
	  else this->pieceLength = 0;

	  elemento = (*dict)[INFO_PIECES];
	  if(elemento != NULL)
	       this->pieces = elemento->beStr;
	  
	  elemento = (*dict)[INFO_PRIVATE];
	  if(elemento != NULL)
	       this->isPrivate = elemento->beInt;
	  else this->isPrivate = 0;

	  elemento = (*dict)[INFO_FILES];
	  if(elemento != NULL);
	       /* Si existe, hay mas de un archivo*/
	  else;
	       /* Si no existe hay un solo archivo */
	       
     }
}
