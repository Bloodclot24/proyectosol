#ifndef SOCKET_H_INCLUDED
#define SOCKET_H_INCLUDED

#include <sys/socket.h>
#include <errno.h>
#include <iostream>
#include <netdb.h>
#include <strings.h>

/* Encapsulacion de sockets */
class Socket{
private:
     int s;  // file descriptor
     struct sockaddr_in direccionDestino;
     int error;                           //Codigo del ultimo error
     std::string ultimoError;             //Descripcion del ultimo error
     struct hostent *host;

     Socket(int fd){
	  s = fd;
	  host=NULL;
	  error = 0;
     }

public:

     /* crea un nuevo socket con la direccion de destino y puerto dados */
     Socket(std::string direccion, int puerto){
	  error = 0;
	  s=socket(AF_INET, SOCK_STREAM, 0);
	  if(s == -1)
	       error=errno;
	  else{
	       host=gethostbyname(direccion.c_str());
	       
	       if(host == NULL)
		    error = errno;

	       direccionDestino.sin_family = AF_INET; /* usa host byte order */
	       direccionDestino.sin_port = htons(puerto); /* usa network byte order */
	       bzero(&(direccionDestino.sin_zero), 8); /* pone en cero el resto */
	  }
     }

     /* Hace que el socket escuche conexiones en su puerto */
     bool enlazar(void){
	  direccionDestino.sin_addr.s_addr = INADDR_ANY;
	  int retorno = bind(s, (struct sockaddr*)&direccionDestino, sizeof(struct sockaddr));
	  if (retorno != 0)
	       error = errno;
	  return esValido();
     }

     /* Acepta un socket que se quiere conectar */
     Socket* aceptar(void){
	  int sckfd = accept(s,NULL,0); 
	  return new Socket(sckfd);
     }

     /* Espera a que algun socket quiera conectarse a este */
     bool escuchar(void){
	  int retorno = listen(s, 20);
	  if(retorno == -1)
	       error = errno;
	  return esValido();
     }
     
     /* Envia un buffer de una cierta longitud por el socket */
     bool enviar(const void *buf, int longitud){
	  int retorno = send(s,buf,longitud,0);
	  if(retorno == -1)
	       error = errno;
	  return esValido();
     }
     
     /* Recibe la cantidad de bytes pedidos en el buffer */
     bool recibir(void *buf, int cuanto){
	  int retorno, acumulado = 0;
	  do{ //recv puede leer menos datos que los pedidos
	       retorno = recv(s, (void*)((int)buf+acumulado), cuanto-acumulado, MSG_WAITALL);
	       acumulado += retorno;
	  }while((acumulado < cuanto) && (retorno > 0));

	  if(retorno == -1)
	       error = errno;
	  return esValido();
     }
     

     /* Conecta al socket a la direccion y puerto dados en el constructor */
     bool conectar(void){
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
     bool cerrar(void){
	  int retorno = close(s);
	  if(retorno == -1)
	       error = errno;
	  return esValido();
     }

     /* Pone un timeout al socket tanto para recepcion de datos como
      * para emision de los mismos. */
     void setTimeout(int seg, int useg){
	  struct timeval tiempo;
	  tiempo.tv_sec= seg;
	  tiempo.tv_usec= useg;
	  setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&tiempo, sizeof(tiempo));
	  tiempo.tv_sec= seg;
	  tiempo.tv_usec= useg;
	  setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (char*)&tiempo, sizeof(tiempo));

     }

     /* Indica si se produjo algun error durante la ultima operacion */
     bool esValido(void){
	  return (error==0);
     }

     /* resetea el estado del socket a valido */
     void revalidar(void){
	  error = 0;
     }

     /* Devuelve la descripcion del ultimo error */
     const std::string& obtenerError(void){
	  ultimoError.clear();
	  ultimoError.insert(0, strerror(error));
	  return ultimoError;
     }

     /* cierra y libera el socket */
     ~Socket(){
	  cerrar();

     }

};

#endif //SOCKET_H_INCLUDED
