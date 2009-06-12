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

public:
     Deque<t>():condition(&mutex){
	  
     }

     t popFront(){
	  t temp;
	  mutex.lock();
	  while(queue.empty())
	       condition.wait();
	  temp = queue.front();
	  queue.pop_front();
	  mutex.unlock();
	  return temp;
     }

     void push(t dato){
	  mutex.lock();
	  queue.push_back(dato);
	  condition.signal();
	  mutex.unlock();
     }

     bool empty(){
	  mutex.lock();
	  bool estado = queue.empty();
	  mutex.unlock();
	  return estado;
     }
     
};

#endif /* DEQUE_H_INCLUDED */
