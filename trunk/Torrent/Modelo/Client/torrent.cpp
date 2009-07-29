#include "torrent.h"
#include "sys/time.h"

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
Torrent::Torrent(const char* fileName, Client* client,
                 BitField* bitfieldGuardado):requestCondition(&requestMutex) {

     this->client= client;

     this->bitField= bitfieldGuardado;

     partsRequested = 0;

     piezasVerificadas = 0;
     timeToAnnounce = 0;
     downloaded = uploaded = corrupt = left = 0;

     ParserBencode parser;
     /* Decodifico todo el .torrent y obtengo una lista con toda la
      * informacion */
     std::list<BeNode*> *info = parser.beDecode(fileName);
     bool check=false;

     this->nombreTorrent = FileManager::obtenerFilename(fileName);

     peersActivos=0;
     estado = STOPPED;
     archivos=NULL;

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
	       }else announceUrlList.push_back(this->announceUrl);

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
     
     this->velSubida= 0;
     this->velBajada= 0;
}

/****************************************************************************/

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
BitField Torrent::getBitField(){
     Lock lock(mutexPiezasPedidas);
     BitField copia(*bitField);
     int size = piezasEnProceso.size();
     while(size > 0){
	  DownloadSlot* ds = piezasEnProceso.popFront();
	  copia.setField(ds->getPieceIndex(),false);
	  piezasEnProceso.push(ds);
	  size--;
     }
     size = listaPiezasPedidas.size();
     while(size > 0){
	  DownloadSlot* ds = listaPiezasPedidas.front();
	  listaPiezasPedidas.pop_front();
	  copia.setField(ds->getPieceIndex(),false);
	  listaPiezasPedidas.push_back(ds);
	  size--;
     }

     return copia;
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
int Torrent::start(){
     mutexEstado.lock();
     if(estado == STOPPED){
	  estado = STARTING;
	  mutexEstado.unlock();
	  /* Comienzo el proceso */
	  Thread::start();
	  return 1;
     }
     mutexEstado.unlock();
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
uint32_t Torrent::rarestFirst() {
     std::list<Peer*>::iterator it;
     const BitField* bitField;
     uint32_t tamanio = this->bitField->getLength();
     uint32_t vectorPiezas[tamanio];
     memset(vectorPiezas,0,tamanio*sizeof(uint32_t));
     uint32_t pieza = 0;
     //Recorro todos los bitfields de todos los peers conectados y
     //guardo en un vector la disponibilidad de las mismas
     for(it =  listaPeersConectados.begin(); it != listaPeersConectados.end(); it++){
	  bitField = (*it)->getBitField();
	  for(uint32_t i=0; i<tamanio; i++){
	       pieza = bitField->getField(i);
	       vectorPiezas[i] += pieza;
	  }
     }

     for(it =  listaPeersEspera.begin(); it != listaPeersEspera.end(); it++){
	  bitField = (*it)->getBitField();
	  for(uint32_t i=0; i<tamanio; i++){
	       pieza = bitField->getField(i);
	       vectorPiezas[i] += pieza;
	  }
     }

     DownloadSlot *ds=NULL;
     mutexPiezasPedidas.lock();
     for(uint32_t i=0;i<piezasEnProceso.size(); i++){
	  ds = piezasEnProceso.popFront();
	  vectorPiezas[ds->getPieceIndex()] = 0;
	  piezasEnProceso.push(ds);
     }
     mutexPiezasPedidas.unlock();

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
int Torrent::announce(){

     /* Si tengo lista de trackers, uso el primero, si no uso el unico
      * que tengo*/
     std::string announceUrl;
     announceUrl=announceUrlList.front();

     /* Creo un request con la direccion del tracker */
     HttpRequest req(announceUrl);

     /* Configuro por defecto el puerto 80, pero si en la direccion
      * del tracker viene incluido el puerto, lo tomo de ahi */
     socket = new Socket(announceUrl, 80);

     /* conecto el socket */
     if(socket->esValido())
	  socket->conectar();

     if(!socket->esValido()){
	  std::string mensaje("Error al conectar: ");
	  mensaje+= socket->obtenerError();
	  /* Si no pude contactar al tracker, envio un mensaje de
	   * error al controlador, elimino el socket y salgo */
	  controlador->agregarMessage(mensaje);
	  delete socket;
	  /* Antes de salir roto la lista de trackers (en caso de que
	   * exista), asi la proxima vez se intenta con uno diferente.
	   */
	  rotarTrackers();
	  return -1;
     }

     receptor = new ThreadReceptor(socket, true);
     emisor = new ThreadEmisor(socket);

     if(receptor == NULL || emisor == NULL){
	  //Error al crear los threads
	  socket->cerrar();
	  delete socket;
	  return -2;
     }

     emisor->comenzar();
     receptor->comenzar();

     /* Agrego algunos parametros al request */
     /* Hash que identifica al torrent */

     left = getTotalSize()-pieceSize*piezasVerificadas;

     req.addParam(REQ_HASH, HttpRequest::UrlEncode(idHash));
     /* 20 bytes que nos identifican */
     req.addParam(REQ_PEER_ID, this->client->getClientId());
     /* Anuncio el puerto que uso para escuchar conexiones */
     req.addParam(REQ_PORT, this->client->getPortIn() );     
     req.addParam(REQ_UPLOADED, this->client->intAstring(uploaded));
     req.addParam(REQ_DOWNLOADED, this->client->intAstring(downloaded));  
     req.addParam(REQ_CORRUPT, this->client->intAstring(corrupt));
     req.addParam(REQ_LEFT, this->client->intAstring(left));     
     req.addParam(REQ_COMPACT, "1");
     /* Cantidad de Peers a pedir */
     req.addParam(REQ_NUMWANT, this->client->getPeersNumWant());

     
     std::string tracker= announceUrlList.front();

     if(client->existeTracker(tracker)) {
	  req.addParam("tracker id", client->trackerId(tracker));
     }

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

     /* Ya tengo la respuesta */

     ParserBencode parser;
     BeNode* primero;
     BeNode* elemento;

     if(resp == NULL){
	  rotarTrackers();
	  return -1;
     }

     /* Parseo la respuesta */
     std::list<BeNode*>* list = parser.beDecode(resp->getContent());

     /* Me fijo que todo valla bien */
     if(list == NULL ||	(primero = list->front()) == NULL	\
	|| primero->typeNode != BE_DICT){

	  controlador->agregarMessage("ERROR: No se pudo decodificar la respuesta del tracker");
	  rotarTrackers();
	  return -1;
     }

     /* La respuesta del tracker consiste en un diccionario, lo obtengo */
     std::map<std::string, BeNode*>* dict = &(primero->beDict->elements);

     /* Extraigo todos los elementos que necesito */
     elemento = (*dict)[DICT_FAIL];
     if(elemento != NULL)
	  controlador->agregarMessage(  "ERROR: " + elemento->beStr );

     elemento = (*dict)[DICT_WARNING];
     if(elemento != NULL)
	  controlador->agregarMessage( "WARNING: " + elemento->beStr );

     elemento = (*dict)[DICT_INTERVAL];
     if(elemento != NULL){
	  announceTime = elemento->beInt;
     }

     elemento = (*dict)[DICT_TRCKID];

     if(elemento != NULL) {
	  std::string tracker= announceUrlList.front();

	  if(!client->existeTracker(tracker)) {
	       client->addTracker(tracker, elemento->beStr);
	  } else {
	       client->modificarIdTracker(tracker,	elemento->beStr);
	  }

     }


     elemento = (*dict)[DICT_PEERS];
     if(elemento != NULL){

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
int Torrent::do_announce(){
     int i=announceUrlList.size();
     /* Me anuncio al tracker */
     while(announce() != 0 && i>=0) i--;
     return i;
}
/****************************************************************************/
void Torrent::run(){
     //Logica. Basicamente pido datos.
     ProtocoloBitTorrent proto;

     controlador->agregarMessage("Informacion: Comenzando el torrent " + nombreTorrent);
     
     struct timeval tiempo;
     if(do_announce() < 0){
	  controlador->agregarMessage("Error: no se pudo conectar con ningun tracker.");
	  mutexEstado.lock();
	  estado = STOPPED;
	  mutexEstado.unlock();
	  return;
     }
     else{
	  /* Si me pude anunciar */
	  gettimeofday(&tiempo, NULL);
	  timeToAnnounce=tiempo.tv_sec + announceTime;
	  mutexEstado.lock();
	  if(estado != STARTING){
	       estado = STOPPED;
	       mutexEstado.unlock();
	       return;
	  }
	  else{
	       estado = DOWNLOADING;
	  }
	  mutexEstado.unlock();


	  while(getEstado() == DOWNLOADING || getEstado() == SEEDING){
	       timeToAnnounce--;

	       mutexPiezasAVerificar.lock();

	       velSubida = 0;
	       velBajada = 0;

	       /* Si habia peers en espera, los vuelvo a insertar */
	       while(listaPeersEspera.size()>0){
		    Peer* peer = listaPeersEspera.front();
		    listaPeersEspera.pop_front();
		    if(peer->conectado){
			 listaPeersConectados.push_back(peer);
			 //velBajada += peer->getVelBajada();
			 //velSubida += peer->getVelSubida();
		    }
		    else delete peer; //eliminarPeer(peer);
	       }
	       
 	       int cantidad= listaPeersActivos.size();
	       while(cantidad>0){
		    Peer* peer = listaPeersActivos.front();
		    listaPeersActivos.pop_front();
		    if(!peer->getTimeOut()){
			 listaPeersActivos.push_back(peer);
		    }
		    else{
			 std::cout << "borro peer por timeout\n";
			 delete peer;
		    }
	       }

	       mutexPiezasAVerificar.unlock();

	       cantidad = listaPeersConectados.size();
	       while(cantidad>0){
		    Peer* peer = listaPeersConectados.front();
		    listaPeersConectados.pop_front();
		    if(peer->conectado){
			 listaPeersConectados.push_back(peer);
			 velBajada += peer->getVelBajada();
			 velSubida += peer->getVelSubida();
		    }
		    else delete peer; //eliminarPeer(peer);
		    cantidad--;
	       }

	       /* Conecto tantos peers como necesite */
	       while(listaPeersConectados.size() < MAX_CONNECTED_PEERS && listaPeers.size() > 0){
		    Peer* peer = listaPeers.front();
		    listaPeers.pop_front();
		    peer->start(idHash);
		    /* me aseguro que el peer no este repetido */
		    listaPeersConectados.remove(peer);
		    listaPeersConectados.push_back(peer);
	       }
	       /* Me fijo si me quedan peers para conectar a futuro */
	       gettimeofday(&tiempo, NULL);
	       if(listaPeers.size()==0 && (uint32_t)tiempo.tv_sec >= timeToAnnounce){
		    /* Si no me quedan y ya puedo anunciarme */
		    //Anunciar
		    controlador->agregarMessage("Anunciando: " + nombreTorrent);
		    if(do_announce()<0){
			 controlador->agregarMessage("Error: no se pudo conectar con ningun tracker.");
		    }
		    else{
			 gettimeofday(&tiempo, NULL);
			 timeToAnnounce=tiempo.tv_sec + announceTime;
		    };
	       }

	       /* Verifico cualquier pieza que pueda estar pendiente */
	       verificarPiezasPendientes();

	       /* Anuncio cualquier pieza que pueda estar pendiente */
	       anunciarPiezasPendientes();

	       if(piezasVerificadas<sizeInPieces){
		    /* Si necesito mas piezas */

		    if(piezasEnProceso.size() > 0){
			 DownloadSlot *ds = NULL;
			 Peer *peer=NULL;

			 // por cada bloque en proceso
			 int size = piezasEnProceso.size();
			 
			 for(int i=0;i<size; i++){
			      Lock lock(mutexPiezasPedidas);
			      ds = piezasEnProceso.popFront();

			      int j=listaPeersConectados.size();
			      while(j>0){
				   peer = listaPeersConectados.front();
				   listaPeersConectados.pop_front();
				   //si encuentro algun peer que la tenga
				   if(peer->havePiece(ds->getPieceIndex()) && peer->getChoke() == 0)
					j=0;
				   else{
					listaPeersConectados.push_back(peer);
					peer = NULL;
					j--;
				   }
			      }
			      if(peer != NULL){
				   //se la asigno para enviar el request
				   ds->setPeer(peer);
				   listaPiezasPedidas.push_back(ds);
				   peer->sendRequest(ds);
				   /* Agrego al peer a la lista de activos */
				   listaPeersActivos.push_back(peer);
				   size--;
				   partsRequested++;
				   peer=NULL;
			      }
			      else{
				   //si no, pruebo con el proximo bloque
				   piezasEnProceso.push(ds);
			      }
			 }
		    }

		    // Si no estoy realizando tantos requests como puedo
		    if(partsRequested < MAX_REQUESTS){
			 //pido otra pieza y agrego los download slots
			 uint32_t index = rarestFirst();
			 mutexBitField.lock();
			 //marco la pieza temporalmente como
			 //disponible para no volver a pedirla
			 bitField->setField(index,true);
			 mutexBitField.unlock();
			 if(index != (uint32_t)-1){
			      uint32_t tamanio = pieceSize;
			      if(index == getSizeInPieces()-1){
				   tamanio = getLastPieceSize();
			      }
			      mutexPiezasPedidas.lock();
			      int contador = DownloadSlot::agregarSlots(piezasEnProceso, index, tamanio, REQUEST_SIZE_DEFAULT);
			      mutexPiezasPedidas.unlock();
			      piezasAVerificar[index] = contador;
			 }
			 else{
			      //no hay mas piezas que podamos pedir
			      //hay que esperar
			      //dormir = true;
			      Lock lock(requestMutex);
			      requestCondition.timedWait(3);
			 }
		    }
	       }
	       else if(getEstado() != SEEDING && getEstado() != STOPPING){
		    /* No necesito mas piezas */
		    /* FIN, salgo y desconecto + seed */
		    mutexEstado.lock();
		    estado = SEEDING;
		    mutexEstado.unlock();
	       }
	  }

	  cleanup();
     }
}

/****************************************************************************/
void Torrent::cleanup(){

     while(listaPeersActivos.size()>0){
	  Peer* peer = listaPeersActivos.front();
	  listaPeersActivos.pop_front();
	  delete peer;
     }
     while(listaPeersEspera.size()>0){
	  Peer* peer = listaPeersEspera.front();
	  listaPeersEspera.pop_front();
	  delete peer;
     }
     int cantidad = listaPeersConectados.size();
     while(cantidad>0){
	  Peer* peer = listaPeersConectados.front();
	  listaPeersConectados.pop_front();
	  delete peer;
	  cantidad--;
     }

     while(listaPeers.size()>0){
	  Peer* peer = listaPeers.front();
	  listaPeers.pop_front();
	  delete peer;
     }

     mutexEstado.lock();
     estado = STOPPED;
     mutexEstado.unlock();
     
     this->velSubida= 0;
     this->velBajada= 0;
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
     peer->setInterested(true);
     peer->setChoke(false);
     //Aviso que ya hay un nuevo Peer en la cola, por si el run()
     //estaba esperando
     requestCondition.signal();
}

/****************************************************************************/
void Torrent::peerUnchoked(Peer* peer){
     Lock lock(requestMutex);
     //Aviso que ya hay un nuevo Peer en la cola, por si el run()
     //estaba esperando
     requestCondition.signal();
}

/****************************************************************************/
void Torrent::peerTransferFinished(Peer* peer, DownloadSlot* ds){
     Lock lock(mutexPiezasAVerificar);
     downloaded += pieceSize;
     if(ds){
	  partsRequested--;
	  piezasAVerificar[ds->getPieceIndex()] -= 1;
	  if(piezasAVerificar[ds->getPieceIndex()]==0)
	       listaPiezasAVerificar.push_back(ds->getPieceIndex());
	  if(getEstado() != DOWNLOADING){
	       return;
	  }
     }
     listaPeersActivos.remove(peer);
     mutexPiezasPedidas.lock();
     if(ds)
	  listaPiezasPedidas.remove(ds);
     mutexPiezasPedidas.unlock();
     listaPeersEspera.push_back(peer);

     delete ds;
}

/****************************************************************************/
void Torrent::verificarPiezasPendientes(){
     Lock lock(mutexPiezasAVerificar);
     Lock lock2(mutexBitField);

     while(listaPiezasAVerificar.size() > 0){
	  uint32_t pieza = listaPiezasAVerificar.front();
	  listaPiezasAVerificar.pop_front();

	  if(validarPieza(pieza)){
	       piezasVerificadas++;
	       bitField->setField(pieza,true);
	       colaPiezasAAnunciar.push(pieza);
	  }
	  else{
	       bitField->setField(pieza,false);
	       piezasAVerificar[pieza] = -1;
	       corrupt += pieceSize;
	  }

     }
}

/****************************************************************************/
void Torrent::anunciarPiezasPendientes(){
     while(colaPiezasAAnunciar.size()>0){
	  uint32_t pieza = colaPiezasAAnunciar.front();
	  colaPiezasAAnunciar.pop();
	  std::list<Peer*>::iterator it;
	  for(it=listaPeersConectados.begin();it!=listaPeersConectados.end();it++)
	       (*it)->have(pieza);

	  for(it=listaPeersActivos.begin();it!=listaPeersActivos.end();it++)
	       (*it)->have(pieza);

	  for(it=listaPeersEspera.begin();it!=listaPeersEspera.end();it++)
	       (*it)->have(pieza);
     }
}

/****************************************************************************/
void Torrent::peerTransferCanceled(Peer* peer, DownloadSlot* ds){
     Lock lock(mutexPiezasAVerificar);
     Lock lock2(mutexPiezasPedidas);

     partsRequested--;
     if(ds != NULL)
	  piezasEnProceso.push(ds);

     if(getEstado() != DOWNLOADING){
	  return;
     }

     listaPeersEspera.push_back(peer);
     listaPeersActivos.remove(peer);
     if(ds != NULL)
	  listaPiezasPedidas.remove(ds);
     requestCondition.signal();
}

/****************************************************************************/
Torrent::~Torrent(){

     while(getEstado() != STOPPED)
	  stop();
     join();

     if(archivos != NULL){
	  while(archivos->size() > 0){
	       TorrentFile* file = archivos->front();
	       archivos->pop_front();
	       delete file;
	  }
	  delete archivos;
	  archivos = NULL;
     }
}

/****************************************************************************/
int Torrent::stop(){
     mutexEstado.lock();

     if(this->estado != STOPPED && this->estado != STOPPING){
	  this->estado= STOPPING;
	  controlador->agregarMessage("Informacion: Frenando el torrent " + nombreTorrent);
	  signal(); //por si se quedo trabado en el announce
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
     return (((double)piezasVerificadas)/(double)sizeInPieces)*100;
}
/****************************************************************************/

void Torrent::abortRequest(DownloadSlot *ds){
     Lock lock(mutexPiezasAVerificar);
     partsRequested--;
     piezasEnProceso.push(ds);
     requestCondition.signal();
}

/****************************************************************************/

void Torrent::agregarPeerExterno(Peer* peer){
     Lock lock(mutexPiezasAVerificar);
     Lock lock2(mutexPiezasPedidas);

     if(getEstado() != DOWNLOADING){
	  delete peer;
	  return;
     }

     listaPeersEspera.push_back(peer);
     requestCondition.signal();
}


/****************************************************************************/
std::list<std::string> Torrent::getListaPeers(){
     mutexListaDireccionsPeers.lock();
     std::list<std::string> copia =listaDireccionesPeers;
     mutexListaDireccionsPeers.unlock();
     return copia;
}

/****************************************************************************/
uint32_t Torrent::getVelocidadSubida() {
	return velSubida;
}

/****************************************************************************/
uint32_t Torrent::getVelocidadBajada() {
	return velBajada;
}
