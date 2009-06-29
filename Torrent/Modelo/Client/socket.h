#ifndef SOCKET_H_INCLUDED
#define SOCKET_H_INCLUDED

#include <sys/socket.h>
#include <errno.h>
#include <iostream>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>

/** 
 * Encapsulacion de sockets.
 */
class Socket{
private:
     int s;			/**< File descriptor */
     struct sockaddr_in direccionDestino; /**< Direccion a donde
					   * conectarse */
     
     int error;			/**< Codigo del ultimo error */
     std::string ultimoError;	/**< Descripcion del ultimo error */
     struct hostent *host;	/**< El host */

     /** 
      * Crea un Socket a partir de un fileDescriptor.
      * 
      * @param fd El file descriptor.
      */
     Socket(int fd){
	  s = fd;
	  host=NULL;
	  error = 0;
     }

     /** 
      * Constructor copia del socket. Se declara privado para evitar
      * conflictos.
      * 
      * @param socket El socket a copiar.
      */
     Socket(const Socket &socket){}

     /** 
      * Constructor copia del socket. Se declara privado para evitar
      * conflictos.
      * 
      * @param socket El socket a copiar.
      */
     Socket operator=(const Socket& socket){ return socket; }
public:

     /** 
      * Crea un nuevo socket con la direccion de destino y puerto
      * dados.
      * 
      * @param direccion Direccion destino.
      * @param puerto Puerto a conectarse.
      */
     Socket(std::string direccion, int puerto);

     /** 
      * Hace que el socket escuche conexiones en su puerto.
      *
      * @return true si tuvo exito y el socket puede aceptar
      * conexiones. false en otro caso.
      */
     bool enlazar(void);

     /** 
      * Acepta un socket que se quiere conectar.  Crea un nuevo Socket
      * con ese file descriptor y lo devuelve.
      * 
      * @return El Socket creado.
      */
     Socket* aceptar(void);

     /** 
      * Espera a que algun socket quiera conectarse a este.
      * 
      * @return true si tuvo exito, false en caso contrario.
      */
     bool escuchar(void);
     
     /** 
      * Envia un buffer de una cierta longitud por el socket.
      * 
      * @param buf El buffer con los datos a enviar.
      * @param longitud La longitud del buffer de datos.
      * 
      * @return true  si tuvo exito, false si no.
      */
     bool enviar(const void *buf, int longitud);
     
     /** 
      * Recibe la cantidad de bytes pedidos en un buffer.
      * 
      * @param buf El buffer donde se almacenan los datos.
      *
      * @param cuanto La cantidad de bytes a leer.
      * 
      * @return La cantidad de bytes almacenados, 0 si el socket se
      * cerro inesperadamente o -1 en caso de error.
      */
     int recibir(void *buf, int cuanto);
     
     /** 
      * Hace que el Socket intente conectarse a su destino.
      * 
      * @return true si pudo conectarse, false si no.
      */
     bool conectar(void);

     /** 
      * Cierra el socket, lo desconecta.
      * 
      * @return true si lo pudo cerrar, false si no.
      */
     bool cerrar(void);

     /** 
      * Pone un timeout al socket tanto para recepcion de datos como
      * para emision de los mismos.
      * 
      * @param seg Segundos de timeout.
      * @param useg Microsegundos de timeout.
      */
     void setTimeout(int seg, int useg);

     /** 
      * Indica si se produjo algun error durante la ultima operacion.
      * 
      * @return true si ocurrio algun error y el socket se encuentra
      * en estado unvalido, false si no.
      */
     bool esValido(void);

     /** 
      * Resetea el estado del socket a valido.
      */
     void revalidar(void);

     /** 
      * Devuelve la descripcion del ultimo error.
      * 
      * @return El string con la descripcion del ultimo error.
      */
     const std::string& obtenerError(void);

     /** 
      * cierra y libera el socket.
      */
     ~Socket();

};

#endif //SOCKET_H_INCLUDED
