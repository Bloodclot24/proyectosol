#ifndef DOWNLOADSLOT_H_INCLUDED
#define DOWNLOADSLOT_H_INCLUDED

#include <queue>
#include "deque.h"
#include <stdint.h>

/**
 * Clase utilizada para llevar la cuenta de las partes que estamos
 * intentando bajar y cuenta la cantidad de veces que fallamos en
 * obtenerla.
 */
class DownloadSlot{
private:
     uint32_t pieceIndex;
     uint32_t offset;
     uint32_t length;
     int errorCount;
     Peer* peer;

public:
     /** 
      * Crea un nuevo DownloadSlot.
      * 
      * @param pieceIndex El numero de pieza a bajar.
      * @param offset El offset dentro de la pieza donde comienza el bloque.
      * @param length La longitud en bytes del bloque.
      * 
      */
     DownloadSlot(uint32_t pieceIndex, uint32_t offset, uint32_t length){
	  this->pieceIndex = pieceIndex;
	  this->offset = offset;
	  this->length = length;
	  errorCount = 0;
     }

     /** 
      * Devuelve el numero de pieza asociada.
      * @return El numero de pieza.
      */
     uint32_t getPieceIndex(){
	  return pieceIndex;
     }
     
     /** 
      * Devuelve el offset del bloque dentro de la pieza.
      * @return El offset.
      */
     uint32_t getOffset(){
	  return offset;
     }

     /** 
      * Devuelve la longitud del bloque.
      * @return La longitud del bloque.
      */
     uint32_t getLength(){
	  return length;
     }
     
     /** 
      * Devuelve la cantidad de veces que fallamos en obtener este
      * bloque.
      * @return La cantidad de intentos fallidos.
      */
     int getErrorCount(){
	  return errorCount;
     }

     /** 
      * Incrementa en 1 el contador de errores.
      */
     void error(){
	  errorCount++;
     }

     /** 
      * Asocia un Peer a este DownloadSlot.
      * 
      * @param peer El peer a asociar.
      */
     void setPeer(Peer *peer){
	  this->peer = peer;
     }
     
     /** 
      * Devuelve el Peer asociado al DownloadSlot.
      * 
      * @return El Peer asociado o NULL si no tiene ninguno.
      */
     Peer* getPeer(){
	  return peer;
     }

     /** 
      * Crea los DownloadSlot necesarios para cubrir todos los bloques
      * del tamao dado en la pieza indicada por index y los agrega en
      * una cola.
      * 
      * @param cola La cola donde se agregan los DownloadSlot creados.
      * @param index El numero de la pieza.
      * @param pieceSize El tamao total de la pieza.
      * @param partSize El tamao de los bloques a crear.
      * 
      * @return El numero de bloques que se agregaron a la cola.
      */
     static int agregarSlots(Deque<DownloadSlot*> &cola, uint32_t index, uint32_t pieceSize, uint32_t partSize){
	  DownloadSlot *ds;
	  int contador = 0;

	  for(uint32_t i=0;i<pieceSize;){
	       uint32_t length;
	       if(pieceSize-i >= partSize)
		    length = partSize;
	       else length = pieceSize-i;
	       
	       ds = new DownloadSlot(index, i, length);
	       i+=length;
	       
	       cola.push(ds);
	       contador++;
	  }
	  return contador;
     }
};


#endif // DOWNLOADSLOT_H_INCLUDED
