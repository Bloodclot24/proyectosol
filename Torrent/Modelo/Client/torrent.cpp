#include "../ParserBencode/parserBencode.h"
#include "../HTTP/HttpRequest.h"
#include "../HTTP/HttpResponse.h"
#include "torrent.h"
#include "mensaje.h"


#define DICT_TRACKER  "announce"
#define DICT_DATE     "creation date"
#define DICT_COMMENTS "comments"
#define DICT_CREATOR  "created by"
#define DICT_ENCODING "encoding"
#define DICT_INFO     "info"

/****************************************************************************/
Torrent::Torrent(std::list<BeNode*>* info){
     BeNode* primero = info->front();

     estado = STOPPED;
     
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

/****************************************************************************/
int Torrent::start(){
     /* Creo un request con la direccion del tracker */
     HttpRequest req(announce);

     /* TODO: OJO */
     socket = new Socket(announce, 80);

     socket->conectar();

     if(!socket->esValido()){
	  std::cout << "Error al conectar: " << socket->obtenerError() << std::endl;
	  delete socket;
	  return -1;
     }

     receptor = new ThreadReceptor(socket);
     emisor = new ThreadEmisor(socket);

     /* Agrego algunos parametros */
     
     req.addParam("info_hash", archivos->front()->getPieces());
     
     /* 20 bytes TODO: pedirselos al cliente */
     req.addParam("peer_id", "-SN1000-abcdefghijkl"); 

     req.addParam("port", "12345");

     req.addParam("uploaded", "0");

     req.addParam("downloaded", "0");
     
     req.addParam("corrupt", "0");

     /* TODO: convertir getTotalSize() a string */
     req.addParam("left", "4154949894");

     req.addParam("compact", "1");

     /* TODO: pedirselo al cliente */
     req.addParam("numwant", "50");

     /* TODO: pedirselo al cliente */
     req.addParam("key", "79m8xvwlyg");

     req.addParam("event", "started");

     /* Obtengo el request completo y lo muestro */
     std::string *request = req.getRequest();
     std::cout << *request << "\n";

     Mensaje *mensaje;

     mensaje->copiarDatos(req.getRequest()->c_str(), req.getRequest()->length());

     emisor->enviarMensaje(mensaje);

     emisor->esperarEnvio();

     /* Libero el string y salgo */
     delete request;

//      HttpResponse resp("HTTP/1.1 200 OK\r\nContent-Length: 574\r\nContent-Type: text/plain\r\n\r\nd8:completei193e10:%20incompletei55e8:intervali1800e12:%40min intervali1%500e5:peers480::privatei0ee");

//      std::cout << "Datos de la respuesta: " << HttpResponse::UrlDecode(resp.getContent()) << "\n";

}


/****************************************************************************/
int Torrent::getTotalSize(){
     std::list<TorrentFile*>::iterator it;
     int acumulador=0;

     for(it=archivos->begin();it!=archivos->end(); it++)
	  acumulador += (*it)->getSize();
     return acumulador;
}
