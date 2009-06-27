#include "HttpRequest.h"

/* Crea un nuevo request HTTP con la url indicada y sin parametros */
/****************************************************************************/
HttpRequest::HttpRequest(std::string direccion):url(direccion){
     primero=true;
}
     
/* Agrega un par parametro<->valor al request */
/****************************************************************************/
void HttpRequest::addParam(std::string param, std::string value){
     if(!primero)
	  request += '&';
     else primero=false;
     request += param;
     request += '=';
     request += value;
}

/* Dado un string codifica los datos en formato URL */
/****************************************************************************/
std::string HttpRequest::UrlEncode(std::string datos){
     std::stringstream codificado;
     unsigned char byte;
     for(size_t i=0;i<datos.length();i++){
	  byte = datos[i];
	  codificado << '%';
	  if(byte<=0x0f)
	       codificado << '0';
	  codificado <<  std::hex << std::uppercase << (int)byte;
     }
     return codificado.str();
}

/* devuelve un string con todos los parametros del request */
/****************************************************************************/
const std::string& HttpRequest::getParams(void){
     return request;
}

/* Devuelve un nuevo string con el request completo. El string
 * debe ser liberado externamente luego de ser utilizado. */
/****************************************************************************/
std::string* HttpRequest::getRequest(void){
     std::string *paquete = new std::string();

     std::string host, dir;

     int auxiliar = strlen("http://");
     if(url.compare(0,auxiliar,"http://") != 0){
	  auxiliar = 0;
     }
	  
     host = url.substr(auxiliar, url.find_first_of(":/",auxiliar)-auxiliar);
     dir  = url.substr(url.find('/',auxiliar));

     *paquete += "GET ";
     *paquete += dir;
     if(request.length() != 0)
	  *paquete += '?';
     *paquete += request;
     *paquete += " HTTP/1.1\r\n";
     *paquete += "User-Agent: TaiyouTorrent/1.00 (rev-1)\r\n";
     *paquete += "Host: ";
     *paquete += host;
     *paquete += "\r\nAccept: */*\r\n";
     *paquete += "Accept-Encoding: text/plain\r\n\r\n";
     //deflate, gzip\r\n\r\n";

     return paquete;
}

