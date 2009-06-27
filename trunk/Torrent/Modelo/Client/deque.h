#ifndef DEQUE_H_INCLUDED
#define DEQUE_H_INCLUDED

#include "threads.h"
#include <deque>

/* Cola threadsafe */
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
     Deque<t>():condition(&mutex),holdCondition(&holdMutex){
	  valida = true;
	  holdCounter = 0;
     }

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

     void push(t dato){
	  mutex.lock();
	  if(valida){
	       queue.push_back(dato);
	       condition.signal();
	  }
	  mutex.unlock();
     }

     bool empty(){
	  mutex.lock();
	  bool estado = queue.empty();
	  mutex.unlock();
	  return estado;
     }

     size_t size(){
	  mutex.lock();
	  size_t tamanio = queue.size();
	  mutex.unlock();
	  return tamanio;
     }

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

     void release(){
	  holdMutex.lock();
	  if(holdCounter > 0){
	       holdCounter--;
	       holdCondition.signal();
	  }
	  holdMutex.unlock();  
     }

     bool isValid(){
	  bool state = false;
	  mutex.lock();
	  state = valida;
	  mutex.unlock();
	  return state;
     }

     ~Deque(){
	  mutex.lock();
	  holdMutex.lock();
	  valida=false;
	  condition.signal();
	  mutex.unlock();
	  while(holdCounter > 0)
	       holdCondition.wait();
	  holdMutex.unlock();
     }
     
};

#endif /* DEQUE_H_INCLUDED */
