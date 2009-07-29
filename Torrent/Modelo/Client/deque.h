#ifndef DEQUE_H_INCLUDED
#define DEQUE_H_INCLUDED

#include "threads.h"
#include <deque>

/** 
  * Cola bloqueante. Es una cola threadsafe que se utiliza para pasar
  * datos entre dos threads distintos. Cuando se quieran sacar datos
  * de la cola, si esta vacia se bloquea y espera o bien a que alguien
  * ponga datosen la cola o a que se llame al destructor.
  */
template <class t>
class Deque{
private:
     Mutex mutex;
     CVariable condition;
     std::deque<t> queue;
     bool valida;
     
public:
     /** 
      * Crea una cola bloqueante. 
      * @return 
      */
     Deque<t>():condition(&mutex){
	  valida = true;
     }

     /** 
      * Saca el primer elemento de la cola y lo devuelve. En caso de
      * no haber mas elementos en la cola, espera a que lleguen nuevos
      * datos.
      *  
      * @return El primer objeto en la cola. Si la cola deja de ser
      * valida mientras se esperan los datos, el objeto retornado
      * puede ser no valido.
      * 
      * @see isValid()
      */
     t popFront(){
	  t temp;
	  mutex.lock();
	  while(queue.empty() && valida)
	       condition.wait();
	  if(valida){
	       temp = queue.front();
	       queue.pop_front();
	  }
	  mutex.unlock();
	  return temp;
     }

     /** 
      * Inserta un elemento en la cola. El objeto se inserta solo si
      * la cola sigue siendo valida.
      * 
      * @param dato El objeto a insertar.
      */
     void push(t dato){
	  mutex.lock();
	  if(valida){
	       queue.push_back(dato);
	       condition.signal();
	  }
	  mutex.unlock();
     }

     /** 
      * Indica si la cola posee mas elementos.
      * 
      * @return false si hay elementos en la cola, true si no.
      */
     bool empty(){
	  mutex.lock();
	  bool estado = queue.empty();
	  mutex.unlock();
	  return estado;
     }

     /** 
      * Devuelve el tamaño de la cola.
      * 
      * @return la cantidad de objetos en espera.
      */
     size_t size(){
	  mutex.lock();
	  size_t tamanio = queue.size();
	  mutex.unlock();
	  return tamanio;
     }


     /** 
      * Indica si es o no valida la cola.
      * @return true si la cola es valida. false si es invalida.
      */
    bool isValid(){
	  bool state = false;
	  mutex.lock();
	  state = valida;
	  mutex.unlock();
	  return state;
    }
     /** 
      * Marca la cola como invalida
      * 
      */
     void invalidate(){
 	  Lock lock(mutex);
 	  valida=false;
 	  condition.signal();
     }

     /** 
      * Destruye la cola. En caso de haber usuarios todavia
      * utilizandola, espera a que terminen.
      */
     ~Deque(){
	  mutex.lock();
	  queue.clear();
	  valida=false;
	  condition.signal();
	  mutex.unlock();
     }
     
};

#endif /* DEQUE_H_INCLUDED */
