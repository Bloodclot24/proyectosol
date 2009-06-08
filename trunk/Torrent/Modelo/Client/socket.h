#ifndef SOCKET_H_INCLUDED
#define SOCKET_H_INCLUDED

#include <sys/socket.h>
#include <errno.h>
#include <iostream>
#include <netdb.h>
#include <strings.h>
#include <string.h>

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
     Socket(std::string direccion, int puerto);

     /* Hace que el socket escuche conexiones en su puerto */
     bool enlazar(void);

     /* Acepta un socket que se quiere conectar */
     Socket* aceptar(void);

     /* Espera a que algun socket quiera conectarse a este */
     bool escuchar(void);
     
     /* Envia un buffer de una cierta longitud por el socket */
     bool enviar(const void *buf, int longitud);
     
     /* Recibe la cantidad de bytes pedidos en el buffer */
     bool recibir(void *buf, int cuanto);
     

     /* Conecta al socket a la direccion y puerto dados en el constructor */
     bool conectar(void);

     /* Cierra el socket, lo desconecta */
     bool cerrar(void);

     /* Pone un timeout al socket tanto para recepcion de datos como
      * para emision de los mismos. */
     void setTimeout(int seg, int useg);

     /* Indica si se produjo algun error durante la ultima operacion */
     bool esValido(void);

     /* resetea el estado del socket a valido */
     void revalidar(void);

     /* Devuelve la descripcion del ultimo error */
     const std::string& obtenerError(void);

     /* cierra y libera el socket */
     ~Socket();

};

#endif //SOCKET_H_INCLUDED
