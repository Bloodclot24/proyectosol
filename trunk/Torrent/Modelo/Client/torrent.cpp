#include "../ParserBencode/parserBencode.h"
#include "../HTTP/HttpRequest.h"
#include "../HTTP/HttpResponse.h"
#include "../SHA1/sha1.h"
#include "torrent.h"
#include "mensaje.h"


#define DICT_TRACKER  "announce"
#define DICT_DATE     "creation date"
#define DICT_COMMENTS "comments"
#define DICT_CREATOR  "created by"
#define DICT_ENCODING "encoding"
#define DICT_INFO     "info"

/****************************************************************************/
Torrent::Torrent(const char* fileName){
     
ParserBencode parser;
     /* Decodifico todo el .torrent y obtengo una lista con toda la
      * informacion */
     std::list<BeNode*> *info = parser.beDecode(fileName);
     
     if(info != NULL){
	  valido = true;

	  BeNode* primero = info->front();
	  
	  estado = STOPPED;
	  
	  if(primero->typeNode == BE_DICT){
	       std::map<std::string, BeNode*>* dict =	\
		    &(primero->beDict->elements);
	       
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
	       if(elemento != NULL){
		    Sha1 hasher;
		    idHash = hasher.ejecutarSha1(			\
			 elemento->buffer->substr(			\
			      elemento->start,				\
			      (elemento->end+1)-elemento->start)	\
			 );
		    /* parseo la estructura con los archivos */
		    archivos = TorrentFile::Parse(elemento);
	       }
	       else valido=false;

	  }
	  
	  /* TODO: liberar cada elemento antes de eliminar */
	  delete info;
     }
     else{
	  valido = false;
     }
}


/****************************************************************************/
int Torrent::start(){
     /* Creo un request con la direccion del tracker */
     HttpRequest req(announce);

//      std::string scrape("announce");
//      size_t pos = announce.find_last_of('/');
//      if(pos != std::string::npos){
// 	  pos = announce.find(scrape, pos);
// 	  if(pos != std::string::npos){
// 	       announce.replace(pos, scrape.length(),"scrape");
// 	  }
//      }


     /* Configuro por defecto el puerto 80, pero si en la direccion
      * del tracker viene incluido el puerto, lo tomo de ahi */
     socket = new Socket(announce, 80);

     /* conecto el socket */
     socket->conectar();

     if(!socket->esValido()){
	  std::cout << "Error al conectar: " <<		\
	       socket->obtenerError() << std::endl;
	  delete socket;
	  return -1;
     }

     receptor = new ThreadReceptor(socket);
     emisor = new ThreadEmisor(socket);

     if(receptor == NULL || emisor == NULL){
	  std::cout << "Error al crear los threads receptor/emisor."	\
		    << std::endl;
	  socket->cerrar();
	  delete socket;
     }
     
     emisor->comenzar();
     receptor->comenzar();

     /* Agrego algunos parametros al request */
     /* Hash que identifica al torrent */
     req.addParam("info_hash", HttpRequest::UrlEncode(idHash));
     
     /* 20 bytes TODO: pedirselos al cliente */
     req.addParam("peer_id", "-SN1000-abcdefghijkl"); 

     /* Tambien pedirselo al cliente */
     req.addParam("port", "12345");
     
     req.addParam("uploaded", "0");
     
     req.addParam("downloaded", "0");
     
     req.addParam("corrupt", "0");

     /* TODO: convertir getTotalSize() a string */
//      req.addParam("left", getTotalSize);

     req.addParam("compact", "1");

     /* TODO: pedirselo al cliente */
     req.addParam("numwant", "50");

     /* TODO: pedirselo al cliente */
     req.addParam("key", "79m8xvwlyg");

     req.addParam("event", "started");

     /* Obtengo el request completo y lo muestro, solo para debugging */
     std::string *request = req.getRequest();
     std::cout << *request << "\nLongitud Total = "	\
	       << request->length() << "\n";

     /* Creo un mensaje con el request */
     Mensaje *mensaje;

     mensaje->copiarDatos(req.getRequest()->c_str(),	\
			  req.getRequest()->length());

     /* envio el request HTTP */
     emisor->enviarMensaje(mensaje);
     emisor->esperarEnvio();
     
     /* espero la respuesta */
     receptor->esperarRecepcion();
     mensaje = receptor->recibirMensaje();
     
     /* Obtengo la respuesta y la muestro, solo para debugging */
     std::cout << "Mensaje obtenido del servidor:\n" << mensaje->getDatos();
     
     HttpResponse resp(mensaje->getDatos());
     
     return 0;
}


/****************************************************************************/
int Torrent::getTotalSize(){
     std::list<TorrentFile*>::iterator it;
     int acumulador=0;

     for(it=archivos->begin();it!=archivos->end(); it++)
	  acumulador += (*it)->getSize();
     return acumulador;
}


/****************************************************************************/
bool Torrent::isValid(){
     return valido;
}
