#ifndef HTTPREQUEST_H_INCLUDED
#define HTTPREQUEST_H_INCLUDED
#include <ctype.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>

/* Clase para construir un request HTTP */
class HttpRequest{
private:
     std::string url;
     std::string request;
     bool primero;
     
public:
     /* Crea un nuevo request HTTP con la url indicada y sin parametros */
     HttpRequest(std::string direccion);
     
     /* Agrega un par parametro<->valor al request */
     void addParam(std::string param, std::string value);

     /* Dado un string codifica los datos en formato URL */
     static std::string UrlEncode(std::string datos);

     /* devuelve un string con todos los parametros del request */
     const std::string& getParams(void);

     /* Devuelve un nuevo string con el request completo. El string
      * debe ser liberado externamente luego de ser utilizado. */
     std::string* getRequest(void);

};

#endif //HTTPREQUEST_H_INCLUDED
