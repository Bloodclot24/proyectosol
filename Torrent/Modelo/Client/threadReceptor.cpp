#include "threadReceptor.h"

/*
 * Crea un nuevo thread para la recepcion de datos mediante un
 * socket.
 */
/****************************************************************************/
ThreadReceptor::ThreadReceptor(Socket *socket, bool http):	\
     pedido(&mutexPedido),estado(&mutexEstado){
     
     this->socket = socket;
     corriendo = false;
     ocupado = false;
     buffer = NULL;
     tamanioBuffer = 0;
     this->http = http; // indica si es o no para recibir informacion http
     response = NULL;
}

/****************************************************************************/
Deque<char>* ThreadReceptor::getColaDeDatos(){
     return &colaDeDatos;
}

/* Si no hay datos en la cola, espera a que haya algo. En el caso de
 * un receptor del tipo http, espera a recibir la respuesta http. */
/****************************************************************************/
void ThreadReceptor::esperarRecepcion(){
     mutexPedido.lock();
     if(http){
	  if(response == NULL)
	       pedido.wait();
     }
     else{
	  while(colaDeDatos.empty())
	       pedido.wait();
     }
     mutexPedido.unlock();
}

/* Activa el thread */
/****************************************************************************/
void ThreadReceptor::comenzar(){
     corriendo = true;
     start();
}

/****************************************************************************/
bool ThreadReceptor::isRunning(){
     return corriendo;
}

/****************************************************************************/
HttpResponse* ThreadReceptor::getResponse(){
     return response;
}

     
/* finaliza el thread y cierra el socket */
/****************************************************************************/
void ThreadReceptor::finalizar(void){
     if(corriendo)
	  corriendo = false;
     socket->setTimeout(0,1);
     socket->cerrar();
}
     
/* Rutina principal del thread */
void ThreadReceptor::run(){
     while(corriendo){
	  if(http){
	       // es un response http
	       std::string datos;
	       std::string auxiliar;
	       char c;

	       bool finalizado = false;

	       socket->recibir(&c, 1);
	       auxiliar.append(1,c);
	       socket->recibir(&c, 1);
	       auxiliar.append(1,c);
	       socket->recibir(&c, 1);
	       auxiliar.append(1,c);

	       while(!finalizado){
		    socket->recibir(&c, 1);
		    auxiliar.append(1,c);
		    if(auxiliar.compare("\r\n\r\n") == 0)
			 finalizado = true;
		    datos.append(1,auxiliar[0]);
		    auxiliar.erase(0,1);
	       }

	       datos.append(1,auxiliar[0]);
	       datos.append(1,auxiliar[1]);
	       datos.append(1,auxiliar[2]);

	       HttpResponse resp(datos);
	       int longitud = resp.getContentLength();
		    
	       if(longitud == -1){
		    char c;
		    while(socket->recibir(&c, 1) > 0){
			 datos.append(1,c);
		    }

	       }
	       else{
		    char *contenido = new char[longitud];
		    socket->recibir(contenido, longitud);
		    datos.append(contenido, longitud);
	       }
	       response = new HttpResponse(datos);
	       corriendo = false;
	  }
	  else{

	       // es un mensaje NO http
	       char c;
	       if(socket->recibir(&c, 1)>0)
		    colaDeDatos.push(c);
	       else{
		    std::cerr << "Se cerro el socket inesperadamente."	\
			      << socket << std::endl;
		    corriendo = false;
	       }
	  }
	  mutexPedido.lock();
	  pedido.signal();
	  mutexPedido.unlock();
     }
}

