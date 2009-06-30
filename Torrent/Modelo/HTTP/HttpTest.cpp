//#include "HttpRequest.h"
//#include "HttpResponse.h"
//
///****************************************************************************/
///*PRUEBA UNITARIA HTTP*/
///****************************************************************************/
//int main(int argc, char** argv){
//
//     /* Creo un nuevo request para cierto traker */
//     HttpRequest req("http://tracker1.transamrit.net:8082/announce");
//     std::string hash("d%0F%E8La%3C%17%F6cU%1D%21%86%89%A6N%8A%EB%EA%BE");
//
//     /* Agrego algunos parametros */
//     req.addParam("info_hash", hash);
//
//     req.addParam("peer_id", "-SN1000-abcdefghijkl"); /* 20 bytes */
//
//     req.addParam("port", "12345");
//
//     req.addParam("uploaded", "0");
//
//     req.addParam("downloaded", "0");
//     
//     req.addParam("corrupt", "0");
//
//     req.addParam("left", "4154949894");
//
//     req.addParam("compact", "1");
//
//     req.addParam("numwant", "50");
//
//     req.addParam("key", "79m8xvwlyg");
//
//     req.addParam("event", "started");
//
//     /* Obtengo el request completo y lo muestro */
//     std::string *request = req.getRequest();
//     std::cout << *request << "\n";
//
//     /* Libero el string y salgo */
//     delete request;
//
//     HttpResponse resp("HTTP/1.1 200 OK\r\nContent-Length: 574\r\nContent-Type: text/plain\r\n\r\nd8:completei193e10:%20incompletei55e8:intervali1800e12:%40min intervali1%500e5:peers480::privatei0ee");
//
//     std::cout << "Datos de la respuesta: " << HttpResponse::UrlDecode(resp.getContent()) << "\n";
//     return 0;
//}
//
///****************************************************************************/
