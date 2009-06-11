#include "../ParserBencode/parserBencode.h"
#include "../HTTP/HttpRequest.h"
#include "../HTTP/HttpResponse.h"
#include "../SHA1/sha1.h"
#include "torrent.h"
#include "mensaje.h"

/* Claves del .torrent */
#define DICT_TRACKER    "announce"
#define DICT_DATE       "creation date"
#define DICT_COMMENTS   "comments"
#define DICT_CREATOR    "created by"
#define DICT_ENCODING   "encoding"
#define DICT_INFO       "info"

/* Claves del response del tracker */
#define DICT_WARNING    "warning message"
#define DICT_FAIL       "failure reason"
#define DICT_INTERVAL   "interval"
#define DICT_MININT     "min interval"
#define DICT_TRCKID     "tracker id"
#define DICT_COMPLETE   "complete"
#define DICT_INCOMPLETE "incomplete"
#define DICT_PEERS      "peers"

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
	  
	  /* libero los elementos antes de eliminar */
	  ParserBencode::beFree(info);
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
//  	  pos = announce.find(scrape, pos);
//  	  if(pos != std::string::npos){
//  	       announce.replace(pos, scrape.length(),"scrape");
//  	  }
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
     req.addParam("left", "1000");

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
     Mensaje *mensaje = new Mensaje;

     mensaje->copiarDatos(req.getRequest()->c_str(),	\
			  req.getRequest()->length());

     /* envio el request HTTP */
     emisor->enviarMensaje(mensaje);
     emisor->esperarEnvio();
     
     /* espero la respuesta */
     receptor->esperarRecepcion();
     mensaje = receptor->recibirMensaje();

	  emisor->finalizar();
	  receptor->finalizar();
	  socket->cerrar();
	  delete emisor;
	  delete receptor;
	  delete socket;
     

     /* Obtengo la respuesta y la muestro, solo para debugging */
     std::cout << "Mensaje obtenido del servidor:\n" << mensaje->getDatos() << std::endl;
     
     std::string datos(mensaje->getDatos(), mensaje->getTamanio());
	       
     HttpResponse resp(datos);

     ParserBencode parser;
     

     std::list<BeNode*>* list = parser.beDecode(resp.getContent());

     BeNode* primero;

     if(list == NULL ||	(primero = list->front()) == NULL	\
	|| primero->typeNode != BE_DICT){

	  std::cout <<							\
	       "ERROR: No se pudo decodificarla respuesta del tracker." \
		    << std::endl;
	  return -1;
     }

     std::map<std::string, BeNode*>* dict = &(primero->beDict->elements);
     
     BeNode* elemento;
	       
     /* Extraigo todos los elementos que necesito */
     elemento = (*dict)[DICT_FAIL];
     if(elemento != NULL)
	  std::cout << "ERROR: " << elemento->beStr << std::endl;
     
     elemento = (*dict)[DICT_WARNING];
     if(elemento != NULL)
	  std::cout << "WARNING: " << elemento->beStr << std::endl;

     elemento = (*dict)[DICT_INTERVAL];
     if(elemento != NULL)
	  std::cout << "INFO: intervalo de requests -> " \
		    << elemento->beInt << "s" << std::endl;

     elemento = (*dict)[DICT_MININT];
     if(elemento != NULL)
	  std::cout << "INFO: intervalo MINIMO de requests -> "	\
		    << elemento->beInt << "s" << std::endl;

     elemento = (*dict)[DICT_TRCKID];
     if(elemento != NULL)
	  std::cout << "INFO: ID TRACKER -> "		\
		    << elemento->beStr << std::endl;

     elemento = (*dict)[DICT_COMPLETE];
     if(elemento != NULL)
	  std::cout << "INFO: Cantidad de veces completo -> "	\
		    << elemento->beInt << std::endl;

     elemento = (*dict)[DICT_INCOMPLETE];
     if(elemento != NULL)
	  std::cout << "INFO: Cantidad de veces incompleto -> "	\
		    << elemento->beInt << std::endl;

     elemento = (*dict)[DICT_PEERS];
     if(elemento != NULL){
	  std::cout << "INFO: Lista de PEERS -> " << std::endl;
	  
	  for(size_t i=0;i<elemento->beStr.length();i+=6){
	       std::cout << ((int)elemento->beStr[i] & 0xff) << "."	\
			 << ((int)elemento->beStr[i+1] & 0xff) << "."	\
			 << ((int)elemento->beStr[i+2] & 0xff) << "."	\
			 << ((int)elemento->beStr[i+3] & 0xff) << ":"	\
			 << ntohs(*(uint16_t*)(elemento->beStr.c_str()+i+4)) \
			 << std::endl;

	       std::string snumero;
	       std::stringstream cvz;
	       cvz << ((int)elemento->beStr[i+0] & 0xff) << "."		\
		   << ((int)elemento->beStr[i+1] & 0xff) << "."		\
		   << ((int)elemento->beStr[i+2] & 0xff) << "."		\
		   << ((int)elemento->beStr[i+3] & 0xff);
	       snumero = cvz.str();
	       
	       Peer *peer = new Peer(snumero,ntohs(*(uint16_t*)(elemento->beStr.c_str()+i+4)), this);
	       peer->start(idHash);

	  }
	  
	  sleep(100000);

     }
     
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

/****************************************************************************/
Torrent::~Torrent(){
     std::list<TorrentFile*>::iterator it;

     if(archivos != NULL){
	  for(it=archivos->begin();it!=archivos->end();it++)
	       delete (*it);
	  
	  delete archivos;
     }
}
