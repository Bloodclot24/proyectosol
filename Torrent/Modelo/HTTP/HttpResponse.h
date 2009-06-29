#ifndef HTTPRESPONSE_H_INCLUDED
#define HTTPRESPONSE_H_INCLUDED

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>

/****************************************************************************/
/* Clase para parsear la respuesta HTTP */
class HttpResponse{

private:
     std::string response;
     int offsetVersion, offsetCode;
     int longitudContenido;
     int offsetContenido;
     
public:
     /**
      * Dado un string con la respuesta, lo parsea obteniendo los
      * datos que nos interesan.
      * 
      * @param respuesta Respuesta a parsear.
      */
     HttpResponse(std::string respuesta);

     /**
      * Devuelve los datos que nos interesan de la respuesta.
      * 
      * @return El contenido de la respuesta. 
      */
     std::string getContent();

     /**
      * Devuelve la longitud del area de datos de la respuesta.
      * 
      * @return La longitud del contenido.
      */
     int getContentLength();

     /**
      * Dado un string codificado en URL, decodifica los datos de 
      * la respuesta.
      * 
      * @param datos Cadena codifica en URL.
      * 
      * @return La cadena con los datos decodificados.
      */
     static std::string UrlDecode(std::string datos);
};

/****************************************************************************/
#endif //HTTPRESPONSE_H_INCLUDED
