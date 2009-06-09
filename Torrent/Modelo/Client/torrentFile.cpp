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

/****************************************************************************/
std::list<TorrentFile*>* TorrentFile::Parse(BeNode* node){

     TorrentFile *newFile = new TorrentFile();
     std::list<TorrentFile*> *fileList = new std::list<TorrentFile*>;
     
     if(node->typeNode == BE_DICT){
	  /* la seccion info debe ser un diccionario */
	  std::map<std::string, BeNode*>* dict = &(node->beDict->elements);
	  BeNode* elemento;
	  /* Extraigo todos los elementos que necesito */

	  elemento = (*dict)[INFO_NAME];
	  if(elemento != NULL)
	       newFile->name = elemento->beStr;
	  
	  elemento = (*dict)[INFO_PIECELEN];
	  if(elemento != NULL)
	       newFile->pieceLength = elemento->beInt;
	  else newFile->pieceLength = 0;

	  elemento = (*dict)[INFO_PIECES];
	  if(elemento != NULL)
	       newFile->pieces = elemento->beStr;
	  
	  elemento = (*dict)[INFO_PRIVATE];
	  if(elemento != NULL)
	       newFile->isPrivate = elemento->beInt;
	  else newFile->isPrivate = 0;

	  fileList->push_back(newFile);

	  elemento = (*dict)[INFO_FILES];
	  if(elemento != NULL){
	       /* Si existe, hay mas de un archivo*/
	       std::list<BeNode*> &lista = elemento->beList->elements;

	       std::list<BeNode*>::iterator it;
	       for(it = lista.begin(); it!=lista.end(); it++){
		    /* Creo un nuevo archivo del torrent */
		    newFile = new TorrentFile();
		    
		    /* obtengo el diccionario de este archivo */
		    dict= &((*it)->beDict->elements);

		    elemento = (*dict)[INFO_LENGTH];
		    if(elemento != NULL)
			 newFile->length = elemento->beInt;
		    else newFile->length = 0;
		    
		    elemento = (*dict)[INFO_MD5];
		    if(elemento != NULL)
			 newFile->md5 = elemento->beStr;

		    elemento = (*dict)[INFO_PATH];
		    if(elemento != NULL)
			 newFile->path = elemento->beStr;
	       }

	       fileList->push_back(newFile);

	  }
	  else{
	       /* Si no existe hay un solo archivo */
	       elemento = (*dict)[INFO_LENGTH];
	       if(elemento != NULL)
		    newFile->length = elemento->beInt;
	       else newFile->length = 0;

	       elemento = (*dict)[INFO_MD5];
	       if(elemento != NULL)
		    newFile->md5 = elemento->beStr;
	  }
	       
     }

     return fileList;
}

/****************************************************************************/
TorrentFile::~TorrentFile(){

     

}
