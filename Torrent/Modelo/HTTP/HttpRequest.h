#ifndef HTTPREQUEST_H_INCLUDED
#define HTTPREQUEST_H_INCLUDED

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>

/****************************************************************************/
/*
 * Clase para construir un request HTTP 
 */
class HttpRequest{

private:
     std::string url;
     std::string request;
     bool primero;
     
public:
     /**
      * Crea un nuevo request HTTP con la url indicada y sin parametros.
      * 
      * @param direccion Direccion a crearle el Http Request.
      */
     HttpRequest(std::string direccion);
     
     /**
      * Agrega un par parametro<->valor al request.
      * 
      * @param param Parametro agregar.
      * @param value Value agregar.
      */
     void addParam(std::string param, std::string value);

     /**
      * Dado un string codifica los datos en formato URL.
      * 
      * @param datos Datos a codificar en formato URL.
      */
     static std::string UrlEncode(std::string datos);

     /**
      * Devuelve un string con todos los parametros del request.
      * 
      * @return Los parametros del request.
      */
     const std::string& getParams();

     /**
      * Devuelve un nuevo string con el request completo. El string
      * debe ser liberado externamente luego de ser utilizado.
      * 
      * @return El request.
      */
     std::string* getRequest();
};

/****************************************************************************/
#endif //HTTPREQUEST_H_INCLUDED
