#ifndef HTTPRESPONSE_H_INCLUDED
#define HTTPRESPONSE_H_INCLUDED

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>

/* Clase para parsear la respuesta HTTP */
class HttpResponse{
private:
     std::string response;
     int offsetVersion, offsetCode;
     int longitudContenido;
     int offsetContenido;
     
public:
     /* dado un string con la respuesta, lo parsea obteniendo los
      * datos que nos interesan */
     HttpResponse(std::string respuesta);

     /* Devuelve los datos que nos interesan */
     std::string getContent();

     /* Devuelve la longitud del area de datos */
     int getContentLength();

     /* Dado un string codificado en URL, decodifica los datos */
     static std::string UrlDecode(std::string datos);
};

#endif //HTTPRESPONSE_H_INCLUDED
