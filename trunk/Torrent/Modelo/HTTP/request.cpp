#include <ctype.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>

class HttpRequest{
private:
     std::string url;
     std::string request;
     bool primero;
     
public:
     /* Crea un nuevo request HTTP con la url indicada y sin parametros */
     HttpRequest(std::string direccion):url(direccion){
	  primero=true;
     }
     
     /* Agrega un par parametro<->valor al request */
     void addParam(std::string param, std::string value){
	  if(!primero)
	       request += '&';
	  else primero=false;
	  request += param;
	  request += '=';
	  request += value;
     }

     /* Dado un string codifica los datos en formato URL */
     static std::string UrlEncode(std::string datos){
	  std::stringstream codificado;
	  unsigned char byte;
	  for(int i=0;i<datos.length();i++){
	       byte = datos[i];
	       if( isalnum(byte) ){
		    codificado << byte;
	       }
	       else{
		    codificado << '%';
		    if(byte<=0x0f)
			 codificado << '0';
		    codificado <<  std::hex << (int)byte;
	       }
	  }
	  return codificado.str();
	  
     }

     /* devuelve un string con todos los parametros del request */
     const std::string& getParams(void){
	  return request;
     }

     /* Devuelve un nuevo string con el request completo. El string
      * debe ser liberado externamente luego de ser utilizado. */
     std::string* getRequest(void){
	  std::string *paquete = new std::string();

	  std::string host, dir;

	  int auxiliar = strlen("http://");
	  if(url.compare(0,auxiliar,"http://") != 0){
	       auxiliar = 0;
	  }
	  
	  host = url.substr(auxiliar, url.find('/',auxiliar)-auxiliar);
	  dir  = url.substr(url.find('/',auxiliar));

	  *paquete += "GET ";
	  *paquete += dir;
	  *paquete += '?';
	  *paquete += request;
	  *paquete += " HTTP/1.1\r\n";
	  *paquete += "User-Agent: TaiyouTorrent/1.00 (rev-1)\r\n";
	  *paquete += "Host: ";
	  *paquete += host;
	  *paquete += "\r\nAccept: */*\r\n";
	  *paquete += "Accept-Encoding: deflate, gzip\r\n\r\n";

	  return paquete;
     }
};

/* Clase para parsear la respuesta HTTP */
class HttpResponse{
     std::string response;
     int offsetVersion, offsetCode;
     int longitudContenido;
     int offsetContenido;
     
public:
     /* dado un string con la respuesta, lo parsea obteniendo los
      * datos que nos interesan */
     HttpResponse(std::string respuesta):response(respuesta){
	  offsetVersion = 0;
	  offsetCode = response.find(' ',0)+1;
	  longitudContenido = atoi(response.c_str()+response.find("Content-Length: ")+strlen("Content-Length: "));
	  offsetContenido = response.find("\r\n\r\n")+4;
     }

     /* Devuelve los datos que nos interesan */
     std::string getContent(){
	  return response.substr(offsetContenido);
     }

     /* Dado un string codificado en URL, decodifica los datos */
     static std::string UrlDecode(std::string datos){
	  std::string decodificado;
	  unsigned char byte;
	  for(int i=0;i<datos.length();i++){
	       byte = datos[i];
	       if( byte != '%' ){
		    decodificado += byte;
	       }
	       else{
		    i++;
		    char auxiliar[3];
		    auxiliar[0] = datos[i];
		    auxiliar[1] = datos[i+1];
		    auxiliar[2] = '\0';

		    decodificado += (char)strtol(auxiliar, NULL, 16);
		    i++;
	       }
	  }
	  return decodificado;
	  
     }

};

int main(int argc, char** argv){

     /* Creo un nuevo request para cierto traker */
     HttpRequest req("http://tracker1.transamrit.net:8082/announce");
     std::string hash("d%0F%E8La%3C%17%F6cU%1D%21%86%89%A6N%8A%EB%EA%BE");

     /* Agrego algunos parametros */
     req.addParam("info_hash", hash);

     req.addParam("peer_id", "-SN1000-abcdefghijkl"); /* 20 bytes */

     req.addParam("port", "12345");

     req.addParam("uploaded", "0");

     req.addParam("downloaded", "0");
     
     req.addParam("corrupt", "0");

     req.addParam("left", "4154949894");

     req.addParam("compact", "1");

     req.addParam("numwant", "50");

     req.addParam("key", "79m8xvwlyg");

     req.addParam("event", "started");

     /* Obtengo el request completo y lo muestro */
     std::string *request = req.getRequest();
     std::cout << *request << "\n";

     /* Libero el string y salgo */
     delete request;

     HttpResponse resp("HTTP/1.1 200 OK\r\nContent-Length: 574\r\nContent-Type: text/plain\r\n\r\nd8:completei193e10:%20incompletei55e8:intervali1800e12:%40min intervali1%500e5:peers480::privatei0ee");

     std::cout << "Datos de la respuesta: " << HttpResponse::UrlDecode(resp.getContent()) << "\n";
     return 0;
}
