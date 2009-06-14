#include "../ParserBencode/parserBencode.h"
#include "../HTTP/HttpRequest.h"
#include "../HTTP/HttpResponse.h"
#include "../SHA1/sha1.h"
#include "torrent.h"
#include "mensaje.h"
#include "client.h"

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

     peersActivos=0;
     estado = STOPPED;
     
     if(info != NULL){
	  valido = true;

	  BeNode* primero = info->front();
	  
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

	  bitField = new BitField(ceil(getTotalSize()/(*archivos->begin())->getPieceLength()));
     }
     else{
	  valido = false;
     }
}

/****************************************************************************/
BitField* Torrent::getBitField(){
     return bitField;
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

     receptor = new ThreadReceptor(socket, true);
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
     req.addParam("peer_id", CLIENT_ID); 

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
     HttpResponse *resp = receptor->getResponse();

	  emisor->finalizar();
	  receptor->finalizar();
	  socket->cerrar();
	  delete emisor;
	  delete receptor;
	  delete socket;
     

     /* Obtengo la respuesta y la muestro, solo para debugging */
	  //std::cout << "Mensaje obtenido del servidor:\n" << r << std::endl;

     ParserBencode parser;

     std::list<BeNode*>* list = parser.beDecode(resp->getContent());

     BeNode* primero;

     if(list == NULL ||	(primero = list->front()) == NULL	\
	|| primero->typeNode != BE_DICT){

	  std::cout <<							\
	       "ERROR: No se pudo decodificar la respuesta del tracker." \
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

	       //creo el nuevo peer con los datos obtenidos
	       Peer *peer = new						\
		    Peer(snumero,					\
			 ntohs(*(uint16_t*)(elemento->beStr.c_str()+i+4)), \
			 this);
	       
	       peer->start(idHash);
	       listaPeers.push_back(peer); // agrego al peer la lista

	  }

	  estado = DOWNLOADING;

	  /* Comienzo el proceso */
	  Thread::start();	  
     }
     
     return 0;
}

/****************************************************************************/
void Torrent::eliminarPeer(Peer* peer){
     if(peer == NULL)
	  return;

     Lock lock(mutexPeers);

     std::list<Peer*>::iterator it;
     bool encontrado = false;

     for(it=listaPeers.begin(); it!=listaPeers.end() && !encontrado; it++){
	  if( (*it) == peer){
	       listaPeers.erase(it);
	       encontrado = true;
	       delete peer;
	  }
     }

     std::cout << "SE BORRO UN PEER\n";
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
uint32_t Torrent::rarestFirst() 
{
     Lock lock(mutexPeers);

     std::list<Peer*>::iterator it=listaPeers.begin();
     const BitField* bitField = (*it)->getBitField();
     uint32_t tamanio = bitField->getLength();
     uint32_t vectorPiezas[tamanio];
     memset(vectorPiezas,0,tamanio*sizeof(uint32_t));
     uint32_t pieza = 0;
     //Recorro todos los bitfields de todos los peers
     //y guardo en un vector la disponibilidad de las
     //mismas
     for(it =  listaPeers.begin(); it != listaPeers.end(); it++){
	  bitField = (*it)->getBitField();
	  for(uint32_t i=0; i<tamanio; i++){
	       pieza = bitField->getField(i);
	       vectorPiezas[i] += pieza;
	  }
     }
	
     uint32_t menor = vectorPiezas[0];
     uint32_t posMenor = 0;
     bool encontrado = false;
     while(!encontrado){
	  //Busco la pieza que esta menor cantidad de veces,
	  //es decir la pieza mas rara.
	  for(uint32_t i=1; i<tamanio; i++){
	       if(menor > vectorPiezas[i]){
		    menor = vectorPiezas[i];
		    posMenor = i;
	       }
	  }
	  //Me fijo si tengo esa parte
	  uint32_t parteExistente = this->bitField->getField(posMenor); 
	  if(parteExistente){
	       //TODO ver lista de partes en proceso.
	       //si la tengo, seteo esa posicion con un valor
	       //muy grande asi no vuelve a ser el menor
	       vectorPiezas[posMenor] = INT_MAX;
	  }else{
	       //Si no la tenia, encontre la pieza mas rara
	       //de las que me faltan y devuelvo la posicion
	       encontrado = true;
	  }
     }
     return posMenor;		
	
}

/****************************************************************************/
TorrentFile* Torrent::obtenerArchivo(uint32_t index)
{
	std::list<TorrentFile*>::iterator it;
	uint32_t tamanioPiezas = 0;
	uint32_t tamanioArchivo = 0;
	uint32_t cantidadBloques = 0;
	uint32_t bloquesAcum = 0;
	bool encontrado = false;
	for(it = archivos->begin() ; it != archivos->end() && !encontrado ; it++){
		tamanioPiezas = (*it)->getPieceLength();
		tamanioArchivo = (*it)->getSize();
		cantidadBloques = ceil( tamanioArchivo / tamanioPiezas);
		bloquesAcum += cantidadBloques;
		if(index <= bloquesAcum){
			encontrado = true;
		}
	}
	if(!encontrado) return NULL;
	return *(--it);
}

/****************************************************************************/
void Torrent::run(){
     while(estado == DOWNLOADING){
	  //Logica. Basicamente pido datos.

	  std::list<Peer*>::iterator it;
// 	  for(it=listaPeers.begin(); it!=listaPeers.end() ; it++){
// 	       (*it)->start(idHash);
// 	       sleep(5);
// 	  }


	  
	  while(true){
	       std::list<Peer*>::iterator it;
	       for(it=listaPeers.begin(); it!=listaPeers.end(); it++){
		    if( (*it)->conectado){
			 ThreadEmisor* emisor= (*listaPeers.begin())->getEmisor();
			 uint32_t indice = rarestFirst();
			 
			 Mensaje *mensaje = new Mensaje();
			 
			 ProtocoloBitTorrent proto;
			 std::string msg = proto.interested();
			 
			 mensaje->copiarDatos(msg.c_str(), msg.length());
			 emisor->enviarMensaje(mensaje);
			 
			 mensaje = new Mensaje();
			 msg = proto.request(indice, 0, 32*1024);
			 mensaje->copiarDatos(msg.c_str(), msg.length());

			 msg = proto.unchoke();
			 mensaje->copiarDatos(msg.c_str(), msg.length());
			 emisor->enviarMensaje(mensaje);

			 std::cout << "Enviado el interested/unchoke.\n";
		    }
	       }

	       std::cout << "No hay peers conectados, reintentando en 10 segundos." << std::endl;
	       sleep(10);
	  }
	  
     }
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