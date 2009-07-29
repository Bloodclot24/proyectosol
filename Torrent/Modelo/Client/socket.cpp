#include "socket.h"
#include <unistd.h>
#include <fcntl.h> 

/* Crea un nuevo socket con la direccion de destino y puerto dados */
/****************************************************************************/
Socket::Socket(std::string direccion, int puerto){
     int puertoFinal=puerto;
     error = 0;
     timeout = 0;
     s=socket(AF_INET, SOCK_STREAM, 0);
     if(s == -1)
	  error=errno;
     else{
	  std::string hostName;
	  size_t inicio,fin,limite;
	  limite = direccion.find('.');
	  inicio = direccion.find("://");
	  fin = direccion.find_first_of(":/", limite);
	       
	  if(inicio != std::string::npos && fin != std::string::npos){
	       hostName= direccion.substr(inicio+3,fin-(inicio+3));
	       if(direccion[fin] == ':'){
		    puertoFinal = atoi(direccion.c_str()+fin+1);
	       }
	  }
	  else hostName = direccion;

	  // "DIRECCION->>>>>>>>>>>>>>>>>>> " << hostName 

	  host=gethostbyname(hostName.c_str());
	       
	  if(host == NULL){
	       error = errno;
	       return;
	  }

	  direccionDestino.sin_family = AF_INET; 
	  /* usa host byte order */
	  
	  direccionDestino.sin_port = htons(puertoFinal); 
	  /* usa network byte order */
	  
	  bzero(&(direccionDestino.sin_zero), 8); 
	  /* pone en cero el resto */
     }
     /* inicializo las velocidades */
     velocidades.velBajada = 0;
     velocidades.velSubida = 0;
     cantVeces = 0;
     tiempoAcumulado = 0;
     bytesAcumulados = 0;
}

/* Hace que el socket escuche conexiones en su puerto */
/****************************************************************************/
bool Socket::enlazar(void){
     direccionDestino.sin_addr.s_addr = INADDR_ANY;
     int retorno = bind(s,					\
			(struct sockaddr*)&direccionDestino,	\
			sizeof(struct sockaddr));
     if (retorno != 0)
	  error = errno;
     return esValido();
}

/* Acepta un socket que se quiere conectar */
/****************************************************************************/
Socket* Socket::aceptar(void){
     int sckfd = accept(s,NULL,0); 
     if (sckfd == -1) return NULL;
     return new Socket(sckfd);
}

/* Espera a que algun socket quiera conectarse a este */
/****************************************************************************/
bool Socket::escuchar(void){
     int retorno = listen(s, 20);
     if(retorno == -1)
	  error = errno;
     return esValido();
}

/* Envia un buffer de una cierta longitud por el socket */
/****************************************************************************/
bool Socket::enviar(const void *buf, int longitud){
     struct timeval tiempo;
     int errorAntes = 0;
     int errorDespues = 0;
     double tiempoAntes = 0;
     double tiempoDespues = 0;
     double tiempoAntesUseg = 0;
     double difTiempos = 0;
     errorAntes = gettimeofday(&tiempo,NULL);
     if(!errorAntes){
	  tiempoAntes = (double)tiempo.tv_sec + ((double)tiempo.tv_usec/1000000);
	  tiempoAntesUseg = tiempo.tv_usec;
     }
     
     int retorno = send(s,buf,longitud,MSG_NOSIGNAL);
     //MSG_NOSIGNAL, evita que se reciba SIGPIPE

     if(retorno == -1)
	  error = errno;
     errorDespues = gettimeofday(&tiempo,NULL);
     if((!errorAntes) && (!errorDespues) && (retorno > 0)){
	  tiempoDespues = (double)tiempo.tv_sec + ((double)tiempo.tv_usec/1000000);
	  
	  difTiempos = tiempoDespues - tiempoAntes;
	  if(difTiempos == 0) difTiempos = tiempo.tv_usec - tiempoAntesUseg;
	  tiempoAcumulado += difTiempos;
	  cantVeces ++;
	  bytesAcumulados += retorno;
	  if(cantVeces == CANT_PROMEDIO){
	       velocidades.velSubida = (double)((bytesAcumulados / tiempoAcumulado) / (double)CANT_PROMEDIO);
	       cantVeces = 0;
	       tiempoAcumulado = 0;
	       bytesAcumulados = 0;
	  }
	  
     }else velocidades.velSubida = 0;  
     
     return esValido();
}
     
/* Recibe la cantidad de bytes pedidos en el buffer */
/****************************************************************************/
int Socket::recibir(void *buf, int cuanto){
     struct timeval tiempo;
     int errorAntes = 0;
     int errorRecv = 0;
     int errorDespues = 0;
     double tiempoAntes = 0;
     double tiempoDespues = 0;
     double tiempoAntesUseg = 0;
     double difTiempos = 0;
     errorAntes = gettimeofday(&tiempo,NULL);
     if(!errorAntes){
	  tiempoAntes = (double)tiempo.tv_sec + ((double)tiempo.tv_usec/1000000);
	  tiempoAntesUseg = tiempo.tv_usec;
     }

     errorRecv = recv(s, buf, cuanto, MSG_WAITALL);

     errorDespues = gettimeofday(&tiempo,NULL);
     if((!errorAntes) && (!errorDespues) && (errorRecv > 0)){
	  tiempoDespues = (double)tiempo.tv_sec + ((double)tiempo.tv_usec/1000000);

	  difTiempos = tiempoDespues - tiempoAntes;
	  if(difTiempos == 0) difTiempos = tiempo.tv_usec - tiempoAntesUseg;
	  tiempoAcumulado += difTiempos;
	  cantVeces ++;
	  bytesAcumulados += errorRecv;
	  if(cantVeces == CANT_PROMEDIO){
	       velocidades.velBajada = (double)((bytesAcumulados / tiempoAcumulado) / (double)CANT_PROMEDIO);
	       cantVeces = 0;
	       tiempoAcumulado = 0;
	       bytesAcumulados = 0;
	  }
     	 	 	
     }else velocidades.velBajada = 0;  
     
     return errorRecv;
}
     

/* Conecta al socket a la direccion y puerto dados en el constructor */
/****************************************************************************/
bool Socket::conectar(void){
     if(!esValido())
	  return esValido();
     if(host == NULL){
	  error = -1;
	  return false;
     }
     direccionDestino.sin_addr = *((struct in_addr *)host->h_addr);

     int retorno=connect(s,(struct sockaddr*)&direccionDestino,sizeof(direccionDestino));

     if(retorno == -1)
	  error = errno;
	  
     return esValido(); 
}

/* Cierra el socket, lo desconecta */
/****************************************************************************/
bool Socket::cerrar(void){
     int retorno=0;
     if(s > 0 ){
	  shutdown(s,SHUT_RDWR);
	  retorno = close(s);
     }
     s=-1;
     if(retorno == -1)
	  error = errno;
     return esValido();
}

/* Pone un timeout al socket tanto para recepcion de datos como
 * para emision de los mismos. */
/****************************************************************************/
void Socket::setTimeout(int seg, int useg){
#ifndef DEBUG
     std::cout << "SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS\n";
     struct timeval tiempo;
     tiempo.tv_sec= seg;
     tiempo.tv_usec= useg;
     setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&tiempo, sizeof(tiempo));
     tiempo.tv_sec= seg;
     tiempo.tv_usec= useg;
     setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (char*)&tiempo, sizeof(tiempo));
#endif
}

/* Indica si se produjo algun error durante la ultima operacion */
/****************************************************************************/
bool Socket::esValido(void){
     return (error==0);
}

/* resetea el estado del socket a valido */
/****************************************************************************/
void Socket::revalidar(void){
     error = 0;
}

/* Devuelve la descripcion del ultimo error */
/****************************************************************************/
const std::string& Socket::obtenerError(void){
     ultimoError.clear();
     ultimoError.insert(0, strerror(error));
     return ultimoError;
}

/* Devuelve la velocidad de bajada */
/****************************************************************************/
uint32_t Socket::getVelBajada(void) {
	
	return velocidades.velBajada;
}
	
/* Devuelve la velocidad de subida */
/****************************************************************************/
uint32_t Socket::getVelSubida(void) {
	
	return velocidades.velSubida;
}

/* Cierra y libera el socket */
/****************************************************************************/
Socket::~Socket(){
     if(s > 0)
	  cerrar();
}
