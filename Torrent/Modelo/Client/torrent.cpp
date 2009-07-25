#include "torrent.h"

/* Claves del .torrent */
#define DICT_TRACKER    "announce"
#define DICT_TRACKERL   "announce-list"
#define DICT_DATE       "creation date"
#define DICT_COMMENTS   "comments"
#define DICT_CREATOR    "created by"
#define DICT_ENCODING   "encoding"
#define DICT_INFO       "info"
#define DICT_INFOA      "azureus_properties"

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
Torrent::Torrent(const char* fileName, BitField* bitfieldGuardado):requestCondition(&requestMutex), 
mutexActualizar() {
     
     this->bitField= bitfieldGuardado;

     partsRequested = 0;

     piezasVerificadas = 0;
     
     ParserBencode parser;
     /* Decodifico todo el .torrent y obtengo una lista con toda la
      * informacion */
     std::list<BeNode*> *info = parser.beDecode(fileName);
     bool check=false;

     this->nombreTorrent = FileManager::obtenerFilename(fileName);

     peersActivos=0;
     estado = STOPPED;
     
     if(info != NULL){
	  
	  // "Torrent Valido"
	  valido = true;

	  BeNode* primero = info->front();
	  
	  if(primero->typeNode == BE_DICT){
	       std::map<std::string, BeNode*> dict =	\
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
//	       elemento2 = dict[DICT_INFOA];
//	       if(elemento2 != NULL){
//	       	if(elemento2->typeNode == BE_DICT){
//	       		dict= (elemento2->beDict->elements);
//	       	}else{
//	       		std::cout << "TORRENT INVALIDOOOOOOOOOOOOOO" << std::endl;
//	       	}
//	       }
//	       
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

		    std::list<TorrentFile*>::iterator it=archivos->begin();
		    if(archivos->size() > 1)
			 it++;
		    for(;it!=archivos->end();it++){
			 TorrentFile *file = *it;
			 
			 FileManager * manager= file->getManager();
			 status=manager->getStatus();
			 if(status == -1){
			      valido = false;
			      //este archivo no se pudo crear.
			 }
			 else if(status == 1){
			      check=1;
			      //el archivo ya existia.
			 }
			 //else -> el archivo no existia.
		    }

	       }
	       else valido=false;

	  }
	  else valido=false;
	  /* libero los elementos antes de eliminar */
	  ParserBencode::beFree(info);
	  delete info;

	  if(valido){

	       pieceSize = (*(archivos->begin()))->getPieceLength();
	       sizeInPieces = ceil((double)getTotalSize()/(double)pieceSize);
			
	       if(bitfieldGuardado == NULL){
		    
		    this->bitField = new BitField(sizeInPieces);
		    
		    //Si alguno de los archivos y ya existia, verifico si hay datos validos
		    if(check){
			 for(uint32_t i=0;i<sizeInPieces;i++){
			      if(validarPieza(i)==1){
				   bitField->setField(i,1);
				   piezasVerificadas++;
			      }
			 }
		    }
	       }
	       else{
		    bitField = bitfieldGuardado;
		    for(uint32_t j = 0;j<bitField->getLength();j++)
			 piezasVerificadas += bitField->getField(j);
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

/****************************************************************************/
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
	  std::string mensaje("Error al conectar: ");
	  mensaje+= socket->obtenerError();
	  controlador->agregarMessage(mensaje);
	  delete socket;
	  rotarTrackers();
	  return -1;
     }

     receptor = new ThreadReceptor(socket, true);
     emisor = new ThreadEmisor(socket);

     if(receptor == NULL || emisor == NULL){
	  //Error al crear los threads
	  socket->cerrar();
	  delete socket;
     }
     
     emisor->comenzar();
     receptor->comenzar();

     /* Agrego algunos parametros al request */
     /* Hash que identifica al torrent */

     req.addParam("info_hash", HttpRequest::UrlEncode(idHash));
     
     /* 20 bytes que nos identifican */
     req.addParam("peer_id", CLIENT_ID); 

     /* Anuncio el puerto que uso para escuchar conexiones */
     req.addParam("port", "1234");
     
     req.addParam("uploaded", "0");
     
     req.addParam("downloaded", "0");
     
     req.addParam("corrupt", "0");

//     TODO: req.addParam("left", getTotalSize);
     req.addParam("left", "1000");

     req.addParam("compact", "1");

     /* Cantidad de Peers a pedir */
     req.addParam("numwant", "50");

     
     req.addParam("key", "79m8xvwlyg");

     req.addParam("event", "started");

     /* Creo un mensaje con el request */
     Mensaje *mensaje = new Mensaje;

     mensaje->copiarDatos(req.getRequest()->c_str(),	\
			  req.getRequest()->length());

     /* envio el request HTTP */
     emisor->enviarMensaje(mensaje);

     emisor->esperarEmision();
     
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

	  controlador->agregarMessage("ERROR: No se pudo decodificar la respuesta del tracker");
	  rotarTrackers();
	  return -1;
     }

     std::map<std::string, BeNode*>* dict = &(primero->beDict->elements);
	       
     /* Extraigo todos los elementos que necesito */
     elemento = (*dict)[DICT_FAIL];
     if(elemento != NULL)
	  controlador->agregarMessage(  "ERROR: " + elemento->beStr );
     
     elemento = (*dict)[DICT_WARNING];
     if(elemento != NULL)
	  controlador->agregarMessage( "WARNING: " + elemento->beStr );

     elemento = (*dict)[DICT_INTERVAL];
     if(elemento != NULL);
//	  controlador->agregarMessage( "INFO: intervalo de requests -> " + elemento->beInt + "s");

     elemento = (*dict)[DICT_MININT];
     if(elemento != NULL);
//	   controlador->agregarMessage("INFO: intervalo MINIMO de requests -> "+ elemento->beInt + "s" );

     elemento = (*dict)[DICT_TRCKID];
     if(elemento != NULL)
	  controlador->agregarMessage( "INFO: ID TRACKER -> "+ elemento->beStr);

     elemento = (*dict)[DICT_COMPLETE];
     if(elemento != NULL)
	  controlador->agregarMessage("INFO: Cantidad de veces completo -> " + elemento->beInt );

     elemento = (*dict)[DICT_INCOMPLETE];
     if(elemento != NULL)
	  controlador->agregarMessage("INFO: Cantidad de veces incompleto -> " + elemento->beInt );

     elemento = (*dict)[DICT_PEERS];
     if(elemento != NULL){
	  controlador->agregarMessage( "INFO: Lista de PEERS -> " );
	  
	  for(size_t i=0;i<elemento->beStr.length();i+=6){
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
	       
	       agregarPeer(peer);
	  }
     }
     return 0;
}

/****************************************************************************/
int Torrent::start(){

     if(estado != DOWNLOADING){
	  if(estado == PAUSED){
	       
	  }
	  else{
	       estado = DOWNLOADING;
	  }
	  /* Comienzo el proceso */
	  Thread::start();	  
	  return 1;
     }
     return 0;
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

     if(!encontrado){
	  listaPeers.push_back(peer); // agrego al peer la lista	  
	  mutexListaDireccionsPeers.lock();
	  listaDireccionesPeers.push_back(peer->getName());
	  mutexListaDireccionsPeers.unlock();
     }
}

/****************************************************************************/
void Torrent::eliminarPeer(Peer* peer){
     if(peer == NULL)
	  return;

     mutexPeers.lock();
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

     
     

     mutexPeers.unlock();
     
     mutexEstado.lock();
     if(listaPeers.size() <= 20  && estado == DOWNLOADING)
	  announce();

     mutexEstado.unlock();
     
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
uint32_t Torrent::readData(char *data, uint32_t index, uint32_t offset, uint32_t size){
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

     if(!encontrado){
	  // "NO SE ENCUENTRA";
	  return -1;
     }

     uint64_t restante = size;
     uint64_t comienzo = bytes-acumulado;
     uint32_t retorno= 0;
     
     while(restante > 0){
	  if(comienzo+restante <= (*it)->getSize()){
	       // Cae toda la parte dentro del archivo
	       (*it)->getManager()->obtenerPieza(data, comienzo, restante);
	       retorno += restante;
	       restante = 0;
	  }
	  else{
	       // Cae una parte adentro y otra parte en el siguiente archivo
	       uint64_t aux=(*it)->getSize()-comienzo;
	       (*it)->getManager()->obtenerPieza(data, comienzo, aux);
	       data += aux;
	       retorno += aux;
	       restante -= aux;;
	       // obtengo el proximo archivo
	       it++;
	       if(it == archivos->end())
		    restante =0; // si no hay mas archivos,no leo mas
	       comienzo = 0;
	  }
     }
     return retorno;
}

/****************************************************************************/
uint64_t Torrent::getTotalSize(){
     std::list<TorrentFile*>::iterator it;
     uint64_t acumulador=0;

     for(it=archivos->begin();it!=archivos->end(); it++){
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

     DownloadSlot *ds=NULL;

     for(uint32_t i=0;i<piezasEnProceso.size(); i++){
	  ds = piezasEnProceso.front();
	  piezasEnProceso.pop();
	  vectorPiezas[ds->getPieceIndex()] = 0;
	  piezasEnProceso.push(ds);
     }

     uint32_t menor = -1;
     uint32_t posMenor = -1;
     //Busco la pieza que esta menor cantidad de veces,
     //es decir la pieza mas rara.
     mutexBitField.lock();
     for(uint32_t i=0;i<tamanio;i++){
	  
	  if(vectorPiezas[i] > 0 && menor > vectorPiezas[i] && 
	     this->bitField->getField(i) == 0){
	       
	       menor = vectorPiezas[i];
	       posMenor = i;
	  }
     }
     mutexBitField.unlock();

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
	return (*it);
}

/****************************************************************************/
int Torrent::validarPieza(uint32_t index){
     if(index > bitField->getLength())
	  return -1;
     char *buffer = new char[pieceSize];
     uint32_t tamanio=0;
     if((tamanio=readData(buffer, index,0,pieceSize))<0){
	  delete[] buffer;
	  return -2;
     }
     int returnValue=0;
     
     Sha1 hasher;
     std::string temp(buffer,tamanio);
     std::string hash = hasher.ejecutarSha1(temp);
     std::string hashes = (*archivos->begin())->getHashes();
     if(hashes.compare(index*20,20,hash) == 0){
	  returnValue = 1;
     }
     else{
	  returnValue = 0;
     }
     
     delete[] buffer;
     return returnValue;     
}

/****************************************************************************/
void Torrent::run(){
     //Logica. Basicamente pido datos.
     ProtocoloBitTorrent proto;
     int i=announceUrlList.size();
      while(announce() != 0 && i>0) i--;
     if(i<=0){
	  controlador->agregarMessage("Error: no se pudo conectar con ningun tracker.");
	  controlador->stop(nombreTorrent);
	  mutexEstado.lock(); 
	  estado = STOPPED;
	  mutexEstado.unlock();
     }
     else{
	  mutexEstado.lock(); 
	  if(estado == DOWNLOADING){
	       
	       mutexPeers.lock();
	       std::list<Peer*>::iterator it;
	       int i;
	       for(i=0,it=listaPeers.begin(); it!=listaPeers.end() && i<30; it++, i++){
		    (*it)->start(idHash);
	       }
	  }
	  else{
	       mutexPeers.lock();
	       estado = DOWNLOADING;
	  }
	  bool dormir = false;
    
	  while(estado == DOWNLOADING){
	       if(peersEnEspera.size()<1 || dormir){
		    mutexPeers.unlock();
		    requestMutex.lock();
		    dormir=false;
		    if(estado == DOWNLOADING){
			 mutexEstado.unlock();
			 requestCondition.wait();
			 mutexEstado.lock();
		    }
		    requestMutex.unlock();
		    mutexPeers.lock();
	       }
	       else{
		    mutexEstado.unlock();
		    mutexPeers.unlock();
		    Lock lock(downloadMutex);
	       
		    // si tenemos piezas en proceso
		    if(piezasEnProceso.size() > 0){
			 DownloadSlot *ds = NULL;
			 Peer *peer=NULL;

			 // por cada bloque en proceso
			 int size = piezasEnProceso.size();
			 for(int i=0;i<size; i++){
			      ds = piezasEnProceso.front();
			      piezasEnProceso.pop();

			      int j=peersEnEspera.size();
			      while(j>0){
				   peer = peersEnEspera.popFront();
				   //si encuentro algun peer que la tenga
				   if(peer->havePiece(ds->getPieceIndex()))
					j=0;
				   else{
					peersEnEspera.push(peer);
					peer = NULL;
					j--;
				   }
			      }
			      if(peer != NULL){
				   //se la asigno para enviar el request
				   ds->setPeer(peer);
				   peer->sendRequest(ds);
				   size--;
				   partsRequested++;
			      }
			      else{
				   //si no, pruebo con el proximo bloque
				   piezasEnProceso.push(ds);
			      }
			 }
		    }
		    mutexPeers.lock();
		    // Si no estoy realizando tantos requests como puedo
		    if(partsRequested < MAX_REQUESTS){
			 //pido otra pieza y agrego los download slots
			 uint32_t index = rarestFirst();
			 mutexBitField.lock();
			 bitField->setField(index,true);
			 mutexBitField.unlock();
			 if(index != (uint32_t)-1){
			      uint32_t tamanio = pieceSize;
			      if(index == getSizeInPieces()-1){
				   tamanio = getLastPieceSize();
			      }

			      int contador = DownloadSlot::agregarSlots(piezasEnProceso, index, tamanio, REQUEST_SIZE_DEFAULT);
			      piezasAVerificar[index] = contador;
			 }
			 else{
			      //no hay mas piezas que podamos pedir
			      //hay que esperar
			      dormir = true;


			 }
		    }
		    else{
			 //no hay mas piezas que podamos pedir
			 //hay que esperar
			 dormir = true;
		    }
		    mutexEstado.lock();
	       }
	  }

	  while(peersEnEspera.size() > 0){
	       peersEnEspera.popFront();
	  }

	  while(listaPeers.size()>0){
	       Peer* peer= listaPeers.front();
	       listaPeers.pop_front();
	       peer->finish();
	       //delete peer;
	  }

	  mutexEstado.unlock();
	  mutexPeers.unlock();
     }
}
/****************************************************************************/
void Torrent::anunciarPieza(uint32_t index){
     std::list<Peer*>::iterator it;
     for(it=listaPeers.begin(); it != listaPeers.end(); it++){
	  if((*it)->conectado)
	       (*it)->have(index);
     }
}

/****************************************************************************/
void Torrent::peerConected(Peer* peer){
     //Envio el mensajede unchoke e interested
     peer->setInterested(true);
     peer->setChoke(false);
}

/****************************************************************************/
void Torrent::peerChoked(Peer* peer){
     Lock lock(requestMutex);
     Lock lock2(mutexPeers);
     for(size_t i = 0 ; i<peersEnEspera.size();i++){
	  Peer *aux = peersEnEspera.popFront();
	  if(aux != peer)
	       peersEnEspera.push(aux);
     }
     peer->setInterested(true);
     peer->setChoke(false);
     //Aviso que ya hay un nuevo Peer en la cola, por si el run()
     //estaba esperando
     requestCondition.signal();     
}

/****************************************************************************/
void Torrent::peerUnchoked(Peer* peer){
     Lock lock(requestMutex);
     Lock lock2(mutexPeers);
     bool encontrado = false;
     //Antes de agregarlo a la lista de peers, me aseguro que no este
     //repetido

     for(size_t i = 0 ; i<peersEnEspera.size()&&!encontrado;i++){
	  Peer *aux = peersEnEspera.popFront();
	  if(aux != peer)
	       peersEnEspera.push(aux);
	  else
	       encontrado = true;
     }
     if(!encontrado)
	  peersEnEspera.push(peer);

     //Aviso que ya hay un nuevo Peer en la cola, por si el run()
     //estaba esperando
     requestCondition.signal();     
}

/****************************************************************************/
void Torrent::peerTransferFinished(Peer* peer, DownloadSlot* ds){
     //OJO, el orden del mutex es importante
     mutexPeers.lock();
     Lock lock(downloadMutex);
     partsRequested--;
     //Antes de agregarlo a la lista de peers, me aseguro que no este
     //repetido
     bool encontrado = false;
     for(size_t i = 0 ; i<peersEnEspera.size()&&!encontrado;i++){
	  Peer *aux = peersEnEspera.popFront();
	  if(aux != peer)
	       peersEnEspera.push(aux);
	  else
	       encontrado = true;
     }
     if(!encontrado)
	  peersEnEspera.push(peer);

     mutexBitField.lock();
	
	 mutexActualizar.lock();
	 controlador->actualizarDownSpeed(nombreTorrent, peer->getVelBajada());	
	 mutexActualizar.unlock();
	 	
		
     piezasAVerificar[ds->getPieceIndex()] -= 1;
     if(piezasAVerificar[ds->getPieceIndex()] == 0){
	  bitField->setField(ds->getPieceIndex(),true);
	  if(validarPieza(ds->getPieceIndex())){
	       piezasVerificadas++;
	       
	       mutexActualizar.lock();
	       controlador->actualizarDone(nombreTorrent, getPorcentaje());
	 	   mutexActualizar.unlock();
	 	       
	       anunciarPieza(ds->getPieceIndex());
	       if(piezasVerificadas >= sizeInPieces){
		    mutexEstado.lock();
		    
		    estado = SEEDING;
		    
	        mutexActualizar.lock();
		    controlador->seed(nombreTorrent);
	 	    mutexActualizar.unlock();
	 		    
		    mutexEstado.unlock();
	       }
	  }
	  else{
	       bitField->setField(ds->getPieceIndex(),false);
	       piezasAVerificar[ds->getPieceIndex()] = -1;
	  }
     }

     delete ds;

     mutexBitField.unlock();

     requestCondition.signal();     
     mutexPeers.unlock();
}

/****************************************************************************/
void Torrent::peerTransferCanceled(Peer* peer, DownloadSlot* ds){
     //OJO, el orden del mutex es importante
     mutexPeers.lock();
     Lock lock(downloadMutex);
     partsRequested--;
     //Antes de agregarlo a la lista de peers, me aseguro que no este
     //repetido
     bool encontrado = false;
     for(size_t i = 0 ; i<peersEnEspera.size()&&!encontrado;i++){
	  Peer *aux = peersEnEspera.popFront();
	  if(aux != peer)
	       peersEnEspera.push(aux);
	  else
	       encontrado = true;
     }
     if(!encontrado)
	  peersEnEspera.push(peer);

     piezasEnProceso.push(ds);

     requestCondition.signal();     
     mutexPeers.unlock();

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
     mutexEstado.lock();
     
     if(this->estado != STOPPED) {
	  this->estado= STOPPED;
	  
	  partsRequested = 0;
	  
	  requestMutex.lock();
	  requestCondition.signal();
	  requestMutex.unlock();
	  mutexEstado.unlock();
	  return 1;
     }
     mutexEstado.unlock();
     return 0;
}

/****************************************************************************/
int Torrent::pause(){

	Lock lock(mutexEstado);
	
	if(this->estado == DOWNLOADING) {
		this->estado= PAUSED;
		return 1; 
	}
	
	return 0;
}

/****************************************************************************/
double Torrent::getPorcentaje(){
     return ((double)piezasVerificadas/(double)sizeInPieces)*100;
}
///****************************************************************************/
//uint32_t Torrent::getVelocidadSubida(){
//     return 0;
//}
//
///****************************************************************************/
//uint32_t Torrent::getVelocidadBajada(){
//     return 0;
//}

/****************************************************************************/
std::list<std::string> Torrent::getListaPeers(){
     mutexListaDireccionsPeers.lock();
     std::list<std::string> copia =listaDireccionesPeers;
     mutexListaDireccionsPeers.unlock();
     return copia;
}
