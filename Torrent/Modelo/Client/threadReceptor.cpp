#include "threadReceptor.h"

/*
 * Crea un nuevo thread para la recepcion de datos mediante un
 * socket.
 */
/****************************************************************************/
ThreadReceptor::ThreadReceptor(Socket *socket, bool http):condHttp(&mutexHttp){
     this->socket = socket;
     ocupado = false;
     this->http = http; // indica si es o no para recibir informacion http
     response = NULL;
}

/****************************************************************************/
Deque<char>* ThreadReceptor::getColaDeDatos(){
     return &colaDeDatos;
}

/* Activa el thread */
/****************************************************************************/
void ThreadReceptor::comenzar(){
     start();
}

/****************************************************************************/
HttpResponse* ThreadReceptor::getResponse(){
     if(response==NULL && http && isRunning()){
	  Lock lock(mutexHttp);
	  condHttp.wait();
     }
     return response;
}
     
/* finaliza el thread y cierra el socket */
/****************************************************************************/
void ThreadReceptor::finish(void){
     if(isRunning()){
	  stop();
	  mutexHttp.lock();
	  condHttp.signal(); //Por si alguien se quedo esperando una respuesta
	  mutexHttp.unlock();

	  socket->setTimeout(0,1);
	  Thread::finish();
	  socket->cerrar();
     }
}
     
/* Rutina principal del thread */
void ThreadReceptor::run(){
     while(isRunning()){
	  if(http){
	       // es un response http
	       std::string datos;
	       std::string auxiliar;
	       char c;

	       bool finalizado = false;

	       if(socket->recibir(&c, 1) <= 0)
		    return;
	       auxiliar.append(1,c);

	       if(socket->recibir(&c, 1) <= 0)
		    return;
	       auxiliar.append(1,c);

	       if(socket->recibir(&c, 1) <= 0)
		    return;
	       auxiliar.append(1,c);

	       while(!finalizado){
		    if(socket->recibir(&c, 1) <= 0)
			 return;

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
	       Lock lock(mutexHttp);
	       response = new HttpResponse(datos);
	       condHttp.signal();
	       stop(); //para que salga del hilo
	  }
	  else{

	       // es un mensaje NO http
	       char c;
	       if(socket->recibir(&c, 1)>0)
		    colaDeDatos.push(c);
	       else{
		    // "Se cerro el socket inesperadamente.
		    stop(); //para que salga del hilo
	       }
	  }
     }
}

