#include "HttpResponse.h"

/* dado un string con la respuesta, lo parsea obteniendo los
 * datos que nos interesan */
HttpResponse::HttpResponse(std::string respuesta):response(respuesta){
     offsetVersion = 0;
     offsetCode = response.find(' ',0)+1;
     longitudContenido = atoi(response.c_str()+ \
			      response.find("Content-Length: ")+ \
			      strlen("Content-Length: "));
	  
     offsetContenido = response.find("\r\n\r\n")+4;
}

/* Devuelve los datos que nos interesan */
std::string HttpResponse::getContent(){
     return response.substr(offsetContenido);
}

int HttpResponse::getContentLength(){
     return longitudContenido;
}

/* Dado un string codificado en URL, decodifica los datos */
std::string HttpResponse::UrlDecode(std::string datos){
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
