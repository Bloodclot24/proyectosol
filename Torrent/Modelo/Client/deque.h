#ifndef DEQUE_H_INCLUDED
#define DEQUE_H_INCLUDED

#include "threads.h"
#include <deque>

/** 
  * Cola bloqueante. Es una cola threadsafe que se utiliza para pasar
  * datos entre dos threads distintos. Para utilizarla, se debe
  * primero llamar al metodo hold(). A partir de este momento, se
  * pueden utilizar las operaciones proporcionadas. Al dejar de
  * utilizarla se debe llamar al metodo release(). Es importante notar
  * que este ultimo, debe ser llamado al menos la misma cantidad de
  * veces que hold(). Si esto no se hace, el objeto no puede ser
  * destruido y el destructor se quedara esperando hasta que se cumpla
  * la condicion.  Inicialmente la cola se encuantra en el estado
  * 'valido'. Solo pasa a ser invalida cuando se invoca al destructor,
  * en cuyo caso, las operaciones normales de la cola retornan
  * inmediatamente.  Adicionalmente, una vez llamado al metodo
  * release(), la cola puede dejar de ser valida automaticamente.
  */
template <class t>
class Deque{
private:
     Mutex mutex;
     CVariable condition;
     std::deque<t> queue;
     bool valida;

     Mutex holdMutex;
     CVariable holdCondition;

     int holdCounter;
     
public:
     /** 
      * Crea una cola bloqueante. 
      * @return 
      */
     Deque<t>():condition(&mutex),holdCondition(&holdMutex){
	  valida = true;
	  holdCounter = 0;
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
      * Avisa a la cola que va a ser utilizada.
      * 
      * @return true si se tuvo exito, o false si falla.
      */
     bool hold(){
	  bool value = false;
	  mutex.lock();
	  holdMutex.lock();
	  if(valida){
	       value = true;
	       holdCounter++;
	  }
	  holdMutex.unlock();
	  mutex.unlock();
	  return value;
     }

     /** 
      * Avisa a la cola que se deja de utilizar.
      */
     void release(){
	  holdMutex.lock();
	  if(holdCounter > 0){
	       holdCounter--;
	       holdCondition.signal();
	  }
	  holdMutex.unlock();  
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
      * Destruye la cola. En caso de haber usuarios todavia
      * utilizandola, espera a que terminen.
      */
     ~Deque(){
	  std::cerr << "chau dequeeeeeeeeeeeeeeeeeeee" << this <<"\n";
	  mutex.lock();
	  holdMutex.lock();
	  valida=false;
	  condition.signal();
	  mutex.unlock();
	  while(holdCounter > 0)
	       holdCondition.wait();
	  std::cerr << "chau dequeeeeeeeeeeeeeeeeeeee  preeeeeeeeeeeunlockkkkk" << this <<"\n";
	  holdMutex.unlock();
	  std::cerr << "chau dequeeeeeeeeeeeeeeeeeeee  postttttttttttunlockkkkk \n";
     }
     
};

#endif /* DEQUE_H_INCLUDED */
