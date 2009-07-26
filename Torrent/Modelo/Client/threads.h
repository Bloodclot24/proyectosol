#ifndef COMMON_THREADS_INCLUDED
#define COMMON_THREADS_INCLUDED

#include <pthread.h>
#include <iostream>
#include <signal.h>

/** 
 * Encapsulacion de las funciones de manejo de Mutex.
 */
class Mutex {

    private:
    pthread_mutex_t mutex;	/**< Mutex */
    
    /** 
     * Copia un Mutex. Privado para evitar que se creen copias
     * externamente.
     */
    Mutex(const Mutex& otherInstance); 
    
    /** 
     * Copia un Mutex. Privado para evitar que se creen copias
     * externamente.
     */
    Mutex& operator=(const Mutex& otherInstance);

    friend class CVariable;	/**< Indica que la clase CVariable
				 * puede acceder a nuestros metodos y
				 * atributos privados */
    
    public:
    /** 
     * Crea un Mutex
     */
    Mutex(){ pthread_mutex_init(&(this->mutex), NULL); }
    
    /** 
     * Bloquea el Mutex, evitando que alguien mas pueda hacerlo.
     */
    void lock(){ pthread_mutex_lock(&(this->mutex)); }

    /** 
     * Desbloquea el Mutex, permitiendo que alguien mas pueda
     * utilizarlo.
     */
    void unlock(){ pthread_mutex_unlock(&(this->mutex)); }
    
    /** 
     * Destruye el Mutex, desbloqueandolo antes de salir.
     */
    virtual ~Mutex() { pthread_mutex_unlock(&(this->mutex)); }

};

/** 
 * Encapsulacion de una Condition Variable.  La forma correcta
 * deutilizarla es bloqueando el Mutex asociado y luego realizar un
 * signal(), broadcast() o wait(). Por ultimo se debe desbloquear el
 * Mutex.
 */
class CVariable {
    
private:
     pthread_cond_t condition;	/**< La Condition Variable */
     Mutex *mutex;		/**< El Mutex asociado a esta
				 * CVariable */
     
public:
     /** 
      * Crea una nueva CVariable asociado a un Mutex.
      * 
      * @param mutex El Mutex a asociar.
      */
     CVariable(Mutex *mutex){
	  this->mutex = mutex; 
	  pthread_cond_init (&condition, NULL); 
     }
     
     /** 
      * Devuelve el Mutex asociado a esta CVariable.
      * 
      * @return El Mutex asociado.
      */
     Mutex* getMutex(){ return mutex; }

     /** 
      * Pide a la CVariable que espere un signal() o broadcast().
      * Para que funcione correctamente debe primero bloquearse el
      * Mutex asociado.
      */
     void wait(){ pthread_cond_wait(&condition, &(mutex->mutex)); }
     
     /** 
      * Pide a la CVariable que emita un signal().  Para que funcione
      * correctamente debe primero bloquearse el Mutex asociado.
      */
     void signal(){ pthread_cond_signal(&condition); }

     /** 
      * Pide a la CVariable que emita un broadcast().  Para que
      * funcione correctamente debe primero bloquearse el Mutex
      * asociado.
      */
     void broadcast(){ pthread_cond_broadcast(&condition); }
     
     /** 
      * Destruye la CVariable, emitiendo antes un broadcast().
      */
     ~CVariable(){pthread_cond_broadcast(&condition);}
};

/** 
 * Clase utilizada para bloquear y desbloquear automaticamente un
 * Mutex. Se utiliza declarando un objeto local de tipo Lock,
 * asociandole un Mutex. Desde este momento el Mutex se encuentra
 * bloqueado. Cuando el objeto sale del scope (se destruye), se libera
 * el Mutex.
 */
class Lock {    
private:
     Mutex &mutex;		/**< Mutex asociado */
     
public:
     
     /** 
      * Crea un nuevo Lock, asignandole un Mutex. Bloquea el Mutex.
      * @param m El Mutex a bloquear.
      */
     Lock(Mutex &m):mutex(m){ mutex.lock(); }
     
     /** 
      * Destruye el objeto y libera el Mutex.
      */
     ~Lock(){ mutex.unlock(); }
};


static void dummyHandler(int signo){
     std::cout << "señal aca esta\n";
     return;
}

/** 
 * Encapsulacion de Threads.
 */
class Thread {
private:
     bool corriendo;		/**< Indica si el Thread esta o no activo. */
     pthread_t thread;		/**< El thread, propiamente dicho. */
     
     /** 
      * Constructor copia. Se define privado para evitar conflictos.
      * @param otherInstance Otra instancia de la cual se genera la
      * copia.
      */
     Thread(const Thread& otherInstance);
     
     /** 
      * Constructor copia. Se define privado para evitar conflictos.
      * @param otherInstance Otra instancia de la cual se genera la
      * copia.
      */
     Thread& operator=(const Thread& otherInstance);
     
     /** 
      * Comienza a correr el Thread. Basicamente, invoca al metodo
      * run().
      * 
      * @param thisPointer Un puntero a this del objeto que quiere
      * comenzar su hilo.
      * 
      * @return NULL
      */
     static void* start_routine(void* thisPointer){
	  Thread* threadPointer = (Thread*) thisPointer;
	  
	  threadPointer->run();
	  return NULL;
     }

     int signalable;
     struct sigaction actions;
     
protected:
     /** 
      * Indica al hilo que debe detenerse.
      */
     void stop(){ corriendo = false; }
     
     /** 
      * Crea un nuevo Thread detenido.
      */
     Thread(){ corriendo = false; thread = 0; 
	  memset(&actions, 0, sizeof(actions));
	  sigemptyset(&actions.sa_mask);
	  actions.sa_flags = 0;
	  actions.sa_handler = &dummyHandler;
	  if(sigaction(SIGUSR1,&actions,NULL)==0)
	       signalable=1;
     }
     
     /** 
      * Metodo virtual puro que debe redefinirse como la funcion
      * principal del Thread.
      */
     virtual void run() = 0;

     
     /** 
      * Espera a que frene el hilo.
      * 
      */
     void finish(){ stop(); }
     
public:

     /** 
      * Comienza el Thread. Invoca al metodo run() y vuelve
      * inmediatamente. A partir de este momento, el hilo ya esta
      * corriendo.
      * @return El valor retornado por pthread_create().
      */
     int start(){ 
	  corriendo=true;
	  return pthread_create(&(this->thread), NULL, Thread::start_routine, this); 
     }

     int signal(){
	  /* if(!signalable) */
/* 	       return -1; */
/* 	  std::cout << "señal aiiiiiiiiiiiiiiiiii\n"; */
	  if(thread!=0)
	       return pthread_kill(thread, SIGUSR1);
	  return 0;
	  
     }

     /** 
      * Indica si el thread sigue o no corriendo.
      *
      * @return true o false si esta o no corriendo el Thread.
      */
     bool isRunning(){ return corriendo; }

     /** 
      * Espera a que el hilo termine.
      */
     void join(){ 
	  if(this->thread != 0){
	       corriendo = false;
	       pthread_join(this->thread, NULL); 
	  }
	  this->thread = 0;
     }

     /** 
      * Destruye el hilo, esperando primero que termine su ejecucion.
      */
     virtual ~Thread(){ stop(); join(); }
};

#endif //COMMON_THREADS_INCLUDED
