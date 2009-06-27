#include "../ParserBencode/parserBencode.h"
#include "../HTTP/HttpRequest.h"
#include "../HTTP/HttpResponse.h"
#include "../SHA1/sha1.h"
#include "torrent.h"
#include "mensaje.h"
#include "client.h"

/* Claves del .torrent */
#define DICT_TRACKER    "announce"
#define DICT_TRACKERL   "announce-list"
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
Torrent::Torrent(const char* fileName, BitField* bitfieldGuardado):requestCondition(&requestMutex){
     
     this->bitField= bitfieldGuardado;
     
     ParserBencode parser;
     /* Decodifico todo el .torrent y obtengo una lista con toda la
      * informacion */
     std::list<BeNode*> *info = parser.beDecode(fileName);
     bool check=false;

     this->nombreTorrent = FileManager::obtenerFilename(fileName);

     peersActivos=0;
     estado = STOPPED;
     
     if(info != NULL){
	  std::cout << "Torrent Valido";
	  valido = true;

	  BeNode* primero = info->front();
	  
	  if(primero->typeNode == BE_DICT){
	       std::map<std::string, BeNode*> &dict =	\
		    primero->beDict->elements;
	       
	       BeNode* elemento;
	       
	       /* Extraigo todos los elementos que necesito */
	       elemento = dict[DICT_TRACKER];
	       if(elemento != NULL)
		    this->announceUrl = elemento->beStr;

	       elemento = dict[DICT_TRACKERL];
	       if(elemento != NULL){
		    armarListaDeTrackers(elemento->beList->elements);
	       }

	       elemento = dict[DICT_DATE];
	       if(elemento != NULL)
		    this->creationDate = elemento->beInt;
	       else this->creationDate = 0;
	       
	       elemento = dict[DICT_COMMENTS];
	       if(elemento != NULL)
		    this->comment = elemento->beStr;
	       
	       elemento = dict[DICT_CREATOR];
	       if(elemento != NULL)
		    this->createdBy = elemento->beStr;
	       
	       elemento = dict[DICT_ENCODING];
	       if(elemento != NULL)
	       this->encoding = elemento->beInt;
	       else this->encoding = 0;
	       
	       /* Informacion de todos los archivos */
	       elemento = dict[DICT_INFO];
	       if(elemento != NULL){
		    Sha1 hasher;
		    idHash = hasher.ejecutarSha1(			\
			 elemento->buffer->substr(			\
			      elemento->start,				\
			      (elemento->end+1)-elemento->start)	\
			 );
		    /* parseo la estructura con los archivos */
		    archivos = TorrentFile::Parse(elemento);
		    int status=0;

		    std::cout << "Verificando estado de los archivos:\n";

		    std::cout << "Lista: "<< archivos << "\n";
		    std::list<TorrentFile*>::iterator it=archivos->begin();
		    if(archivos->size() > 1)
			 it++;
		    for(;it!=archivos->end();it++){
			 TorrentFile *file = *it;
			 
			 FileManager * manager= file->getManager();
			 std::cout << "Archivo " << file << " , manager" << manager << "\n";
			 status=manager->getStatus();
			 if(status == -1){
			      valido = false;
			      std::cout << "este archivo molesta\n";
			 }
			 else if(status == 1){
			      check=1;
			      std::cout << "este archivo ya existia\n";
			 }
			 else{
			      std::cout << "este archivo NO existia\n";
			 }
		    }

	       }
	       else valido=false;

	  }
	  else valido=false;
	  /* libero los elementos antes de eliminar */
	  ParserBencode::beFree(info);
	  delete info;

	  if(valido){
			
			if(!bitField) {
			
				pieceSize = (*(archivos->begin()))->getPieceLength();
		       	sizeInPieces = ceil(getTotalSize()/pieceSize);
		       
		       	std::cout << "PieceSize" << pieceSize << " SizeInPieces "<< sizeInPieces << "\n";
	
		       	this->bitField = new BitField(sizeInPieces);
		       
		       	//Si alguno de los archivos y ya existia, verifico si hay datos validos
		       	if(check){
			    	std::cout << "Los archivos ya existian, verificando ........\n" ;
			    	for(int i=0;i<sizeInPieces;i++){
				 		if(validarPieza(i)==1)
				      		bitField->setField(i,1);
			    	}
		       	}
			}   
	  	}
     }
     else{
	  valido = false;
     }
}

void  Torrent::armarListaDeTrackers(const std::list<BeNode*> &listaLista){
     std::list<BeNode*>::const_iterator it;
     for(it = listaLista.begin();it != listaLista.end(); it++){
	  BeNode* elemento = *it;
	  if(elemento->typeNode == BE_LIST){
	       const std::list<BeNode*> listaInterna = elemento->beList->elements;
	       std::list<BeNode*>::const_iterator itInterno;
	       for(itInterno = listaInterna.begin();itInterno != listaInterna.end(); itInterno++){
		    BeNode* elementoInterno = *itInterno;
		    if(elementoInterno->typeNode == BE_STR){
			 announceUrlList.push_back(elementoInterno->beStr);
		    }
	       }
	  }
     }
}

/****************************************************************************/
BitField* Torrent::getBitField(){
     return bitField;
}

void Torrent::rotarTrackers(){
     if(announceUrlList.size() > 0){
	  std::string tracker = announceUrlList.front();
	  announceUrlList.pop_front();
	  announceUrlList.push_back(tracker);
     }
}

/****************************************************************************/
int Torrent::announce(){

     std::string announceUrl;
     if(announceUrlList.size() > 0){
	  announceUrl=announceUrlList.front();
     }
     else announceUrl = this->announceUrl;

     /* Creo un request con la direccion del tracker */
     HttpRequest req(announceUrl);

     /* Configuro por defecto el puerto 80, pero si en la direccion
      * del tracker viene incluido el puerto, lo tomo de ahi */
     socket = new Socket(announceUrl, 80);

     /* conecto el socket */
     socket->conectar();

     if(!socket->esValido()){
	  std::cout << "Error al conectar: " <<		\
	       socket->obtenerError() << std::endl;
	  delete socket;
	  rotarTrackers();
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

     std::cout << "Hash sin URLENCODE " << idHash << "\n";
     std::cout << "Hash con URLENCODE " << HttpRequest::UrlEncode(idHash) << "\n";

     req.addParam("info_hash", HttpRequest::UrlEncode(idHash));
     
     /* 20 bytes que nos identifican */
     req.addParam("peer_id", CLIENT_ID); 

     /* Anuncio el puerto que uso para escuchar conexiones */
     req.addParam("port", "1234");
     
     req.addParam("uploaded", "0");
     
     req.addParam("downloaded", "0");
     
     req.addParam("corrupt", "0");

//      req.addParam("left", getTotalSize);
     req.addParam("left", "1000");

     req.addParam("compact", "1");

     /* Cantidad de Peers a pedir */
     req.addParam("numwant", "50");

     /* TODO: pedirselo al cliente */
     req.addParam("key", "79m8xvwlyg");

     req.addParam("event", "started");

     /* Creo un mensaje con el request */
     Mensaje *mensaje = new Mensaje;

     std::cout << "REQUEST (PARA VERO)\n: "<< req.getRequest()->c_str()<< "\n";

     mensaje->copiarDatos(req.getRequest()->c_str(),	\
			  req.getRequest()->length());

     /* envio el request HTTP */
     emisor->enviarMensaje(mensaje);

     emisor->esperarEmision();
     sleep(2);
     
     /* recibo la respuesta */
     HttpResponse *resp = receptor->getResponse();

     emisor->finish();
     receptor->finish();
     socket->cerrar();
     delete emisor;
     delete receptor;
     delete socket;

     ParserBencode parser;
     BeNode* primero;
     BeNode* elemento;

     if(resp == NULL){
	  rotarTrackers();
	  return -1;
     }

     std::list<BeNode*>* list = parser.beDecode(resp->getContent());

     if(list == NULL ||	(primero = list->front()) == NULL	\
	|| primero->typeNode != BE_DICT){

	  std::cout <<							\
	       "ERROR: No se pudo decodificar la respuesta del tracker." \
		    << std::endl;
	  rotarTrackers();
	  return -1;
     }

     std::map<std::string, BeNode*>* dict = &(primero->beDict->elements);
	       
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
	      
	       Peer *peer = new					\
		    Peer(snumero,					\
			 ntohs(*(uint16_t*)(elemento->beStr.c_str()+i+4)), \
			 this);
	      
	       agregarPeer(peer);
	  }
     }
     return 0;
}

/****************************************************************************/
int Torrent::start(){

     estado = DOWNLOADING;
  
     /* Comienzo el proceso */
     Thread::start();	  

     return 1;
}

/****************************************************************************/
void Torrent::agregarPeer(Peer* peer){
     if(peer == NULL)
	  return;

     Lock lock(mutexPeers);
     std::list<Peer*>::iterator it;
     bool encontrado = false;

     for(it=listaPeers.begin(); it!=listaPeers.end() && !encontrado; it++){
	  if( (*it)->getName() == peer->getName()){
	       encontrado = true;
	  }
     }

     if(!encontrado)
	  listaPeers.push_back(peer); // agrego al peer la lista	  
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
	       //delete peer;
	       break;
	  }
     }

     encontrado = false;
     for(it=listaPeers.begin(); it!=listaPeers.end() && !encontrado; it++){
	  if( !(*it)->conectado){
	       (*it)->start(idHash);
	       encontrado = true;
	  }
     }

     std::cout << "SE ELIMINO UN PEEEEEER\n";
     std::cout << "QUEDAN " << listaPeers.size() << "\n";

     //if(!encontrado)
	  //announce();
}

/****************************************************************************/
int Torrent::writeData(const char* data, uint32_t index, uint32_t offset, uint32_t size){

     std::list<TorrentFile*>::iterator it;

     uint64_t acumulado=0;
     uint64_t bytes = index*pieceSize+offset;
     bool encontrado = false;


     // Busco en que archivo caen los datos
     it=archivos->begin();
     if(archivos->size() > 1)
	  it++;
     while(it != archivos->end() && !encontrado){
	  if(acumulado+(*it)->getSize() >= bytes)
	       encontrado = true;
	  else{
	       acumulado += (*it)->getSize();
	       *it++;
	  }
     }

     if(!encontrado)
	  return -1;


     uint64_t restante = size;
     uint64_t comienzo = bytes-acumulado;
     
     while(restante > 0){
	  if(comienzo+restante <= (*it)->getSize()){
	       // Cae toda la parte dentro del archivo
	       (*it)->getManager()->agregarPieza(data, comienzo, restante);
	       restante = 0;
	  }
	  else{
	       // Cae una parte adentro y otra parte en el siguiente archivo
	       uint64_t aux=(*it)->getSize()-comienzo;
	       (*it)->getManager()->agregarPieza(data, comienzo, aux);
	       data += aux;
	       restante -= aux;;
	       // obtengo el proximo archivo
	       it++;
	       if(it == archivos->end())
		    restante =0; // si no hay mas archivos,no escribo mas
	       comienzo = 0;
	  }
     }
     return 0;
}

/****************************************************************************/
int Torrent::readData(char *data, uint32_t index, uint32_t offset, uint32_t size){
     std::list<TorrentFile*>::iterator it;

     uint64_t acumulado=0;
     uint64_t bytes = index*pieceSize+offset;
     bool encontrado = false;

     // Busco en que archivo caen los datos
     it=archivos->begin();
     if(archivos->size() > 1)
	  it++;
     while(it != archivos->end() && !encontrado){
	  if(acumulado+(*it)->getSize() >= bytes)
	       encontrado = true;
	  else{
	       acumulado += (*it)->getSize();
	       *it++;
	  }
     }

     if(!encontrado)
	  return -1;

     uint64_t restante = size;
     uint64_t comienzo = bytes-acumulado;
     
     while(restante > 0){
	  if(comienzo+restante <= (*it)->getSize()){
	       // Cae toda la parte dentro del archivo
	       (*it)->getManager()->obtenerPieza(data, comienzo, restante);
	       restante = 0;
	  }
	  else{
	       // Cae una parte adentro y otra parte en el siguiente archivo
	       uint64_t aux=(*it)->getSize()-comienzo;
	       (*it)->getManager()->obtenerPieza(data, comienzo, aux);
	       data += aux;
	       restante -= aux;;
	       // obtengo el proximo archivo
	       it++;
	       if(it == archivos->end())
		    restante =0; // si no hay mas archivos,no leo mas
	       comienzo = 0;
	  }
     }
     return 0;
}

/****************************************************************************/
uint64_t Torrent::getTotalSize(){
     std::list<TorrentFile*>::iterator it;
     uint64_t acumulador=0;

     for(it=archivos->begin();it!=archivos->end(); it++){
	  std::cout << "Tamanio = " << (*it)->getSize() << "\n";
	  acumulador += (*it)->getSize();
     }
     return acumulador;
}


/****************************************************************************/
bool Torrent::isValid(){
     return valido;
}

/****************************************************************************/
uint64_t Torrent::obtenerByteOffset(uint32_t index){
     std::list<TorrentFile*>::iterator it;
     uint32_t tamanioPiezas = 0;
     uint32_t tamanioArchivo = 0;
     uint32_t bytesAcum = 0;
     bool encontrado = false;
     for(it = archivos->begin() ; it != archivos->end() && !encontrado ; it++){
	  tamanioPiezas = (*it)->getPieceLength();
	  tamanioArchivo = (*it)->getSize();

	  if(index*tamanioPiezas < (bytesAcum+tamanioArchivo)){
	       encontrado = true;
	  }
	  else{
	       bytesAcum+=tamanioArchivo;
	  }
     }

     if(!encontrado) return -1;

     return (index*tamanioPiezas-bytesAcum);
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
	       BitField *fields = piezasEnProceso[posMenor];
	       if(fields != NULL){
		   	//TODO ver lista de partes en proceso.
	       	//si la tengo, seteo esa posicion con un valor
	       	//muy grande asi no vuelve a ser el menor
	       	vectorPiezas[posMenor] = INT_MAX;
	       }
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
	uint32_t bytesAcum = 0;
	bool encontrado = false;
	for(it = archivos->begin() ; it != archivos->end() && !encontrado ; it++){
		tamanioPiezas = (*it)->getPieceLength();
		tamanioArchivo = (*it)->getSize();
		if(tamanioArchivo == 0)
		     continue;
		if(index*tamanioPiezas < (bytesAcum+tamanioArchivo)){
		     encontrado = true;
		     break;
		}
		else{
		     bytesAcum+=tamanioArchivo;
		}
	}
	if(!encontrado) return NULL;
	return *(--it);
}

/****************************************************************************/
int Torrent::validarPieza(uint32_t index){
     if(index > bitField->getLength())
	  return -1;
     char *buffer = new char[pieceSize];
     if(readData(buffer, index,0,pieceSize)!=0){
	  delete[] buffer;
	  return -2;
     }
     
     Sha1 hasher;
     std::string temp(buffer,pieceSize);
     std::string hash = hasher.ejecutarSha1(temp);
     std::string hashes = (*archivos->begin())->getHashes();
     if(hashes.compare(index*20,20,hash) == 0){
	  std::cout << "La pieza es "<< index << " valida.\n";
	  bitField->setField(index,1);
     }
     else
	  std::cout << "La pieza es "<< index << " invalida.\n";

     delete[] buffer;
     return 0;     
}

/****************************************************************************/
void Torrent::run(){
     //Logica. Basicamente pido datos.
     ProtocoloBitTorrent proto;

//////////////////////     while(announce() != 0);
     Peer *peer = new							\
	  Peer("localhost",						\
	       6881,							\
	       this);
     
     agregarPeer(peer);

     mutexPeers.lock();
       std::list<Peer*>::iterator it;
       int i;
       for(i=0,it=listaPeers.begin(); it!=listaPeers.end() && i<30; it++, i++){
  	  (*it)->start(idHash);
       }
     
       while(getEstado() == DOWNLOADING){
	    if(peersEnEspera.size()<1){
		 mutexPeers.unlock();
		 requestMutex.lock();
		 requestCondition.wait();
		 requestMutex.unlock();
		 mutexPeers.lock();
	    }
	    else{
		 std::cout << "Puedo realizar un request. (" << peersEnEspera.size()<<")\n";
		 sleep(1);
		 Lock lock(downloadMutex);
		 
		 static uint32_t index = 0; // rarestFirst();
		 std::cout << "Realizo un request de la pieza " << index <<".\n";
		 std::cout << "Peers en la cola de espera: " << peersEnEspera.size() << "\n";
		 
		 
		 BitField *fields = piezasEnProceso[index];
		 if(fields != NULL){
		      std::cout << "La pieza esta siendo procesada en algun lado\n";
		      uint32_t inicio=-1,fin=-1;
		      uint32_t size;
		      uint32_t j;
		      for(j=0;fields->getField(j) != 0 && j < fields->getLength();j++);
		      inicio = j;
		      for(j=inicio+1;fields->getField(j) != 1 && j < fields->getLength();j++);
		      fin = j;
		      
		      if(inicio >= fields->getLength()){
			   std::cout << "UPAAAAA, nos pasamos de pieza, vamos por la que sigue!!\n";
			   piezasEnProceso[index]=NULL;
			   //delete fields;
			   bitField->setField(index,1);
			   //Me fijo que la pieza sea valida antes de continuar
			   if(validarPieza(index))
				//Anunciar el have
				anunciarPieza(index);
			   
			   index++;
			   inicio=0;
			   fin=REQUEST_SIZE_DEFAULT;
			   
		      }
		      
		      std::cout << "INICIO: " << inicio << " FIN: " << fin << std::endl;
		      size = fin-inicio;
		      
		      size_t i = peersEnEspera.size();
		      Peer *peer;
		      while(i>0){
			   peer = peersEnEspera.popFront();
			   if(peer->havePiece(index))
				i=0;
			   else{
				peersEnEspera.push(peer);
				peer = NULL;
				i--;
			   }
		      }
		      if(peer != NULL){
			   if(size>REQUEST_SIZE_DEFAULT)
				size = REQUEST_SIZE_DEFAULT;
			   for(uint32_t i=0;i<size;i++)
				fields->setField(inicio+i,1);
			   peer->sendRequest(index,inicio,size);
		      }
		      else{
			   index++;
		      }
		 }
		 else{
		      std::cout << "No estamos procesando la pieza.\n";
		      size_t i = peersEnEspera.size();
		      Peer *peer;
		      while(i>0){
			   peer = peersEnEspera.popFront();
			   if(peer->havePiece(index))
				i=0;
			   else{
				peersEnEspera.push(peer);
				peer = NULL;
				i--;
			   }
		      }
		      if(peer != NULL){
			   //Si encontramos al peer que tiene la pieza
			   //Creo un bitfield con la cantidad de bits
			   //necesarias para representar una pieza
			   fields = new BitField((*archivos->begin())->getPieceLength());
			   piezasEnProceso[index] = fields;
			   std::cout << "Se envia el request de la pieza " << index << std::endl;
			   for(uint32_t i=0;i<REQUEST_SIZE_DEFAULT;i++)
				fields->setField(i,1);
			   peer->sendRequest(index,0,REQUEST_SIZE_DEFAULT);
		      }
		      else{
			   index++;
		      }
		    
		 }
	    }
       }
}

/****************************************************************************/
void Torrent::anunciarPieza(uint32_t index){
     std::list<Peer*>::iterator it;
     Lock lock(mutexPeers);

     for(it=listaPeers.begin(); it != listaPeers.end(); it++){
	  if((*it)->conectado)
	       (*it)->have(index);
     }
}

/****************************************************************************/
void Torrent::peerConected(Peer* peer){
     std::cout << "Enviando Interested, unchoked\n";
     peer->setInterested(true);
     peer->setChoke(false);
}

/****************************************************************************/
void Torrent::peerChoked(Peer* peer){
     Lock lock(requestMutex);
     for(size_t i = 0 ; i<peersEnEspera.size();i++){
	  Peer *aux = peersEnEspera.popFront();
	  if(aux != peer)
	       peersEnEspera.push(aux);
     }
     peer->setInterested(true);
     peer->setChoke(false);
     std::cout << "Seal de choke ("<< peersEnEspera.size() <<") \n";
     requestCondition.signal();     
}

/****************************************************************************/
void Torrent::peerUnchoked(Peer* peer){
     Lock lock(requestMutex);
     peersEnEspera.push(peer);
     std::cout << "Seal de unchoke ("<< peersEnEspera.size() <<") \n";
     requestCondition.signal();     
}

/****************************************************************************/
Torrent::~Torrent(){
     std::list<TorrentFile*>::iterator it;

     if(archivos != NULL){
	  while(archivos->size() > 0){
	       TorrentFile* file = archivos->front();
	       archivos->pop_front();
	       delete file;
	  }
	  
	  delete archivos;
     }
}

/****************************************************************************/
int Torrent::stop(){
     Lock lock(mutexPeers);
     //TODO: ver tema mutex estado
     this->estado= STOPPED;
     
     while(listaPeers.size()>0){
	  Peer* peer = listaPeers.front();
	  listaPeers.pop_front();
	  peer->finish();
	  delete peer;
     }
     
     return 1;
}

/****************************************************************************/
int Torrent::pause(){
	
	//TODO: ver tema mutex
	this->estado= PAUSED;
	return 1; 
}

/****************************************************************************/
