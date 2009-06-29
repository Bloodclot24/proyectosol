#ifndef PARSERBENCODING_H_
#define PARSERBENCODING_H_

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <map>

#define START_INT "i"
#define START_LIST "l"
#define START_DICT "d"
#define SEPARATOR ":"
#define END "e"

enum BeType {BE_END, BE_STR, BE_INT, BE_LIST, BE_DICT};

struct BeNode;

typedef struct {
     std::list<BeNode*> elements;
} BeList;
		
typedef struct {
     std::map<std::string,BeNode*> elements;
} BeDict;

typedef struct BeNode {
     BeType typeNode;
     std::string beStr;
     uint64_t beInt;
     BeList*	beList;
     BeDict*	beDict;
     const std::string *buffer;
     std::string::size_type start;
     std::string::size_type end;
} BeNode;

/****************************************************************************/
class ParserBencode {

	private:  
		/**
		 * Funciones internas para decodificar los distintos formatos.
		 */
	    std::string beDecodeStr(const std::string *stringStr, 
				                std::string::size_type startPosition,
				                std::string::size_type &endPosition);
	     
	    uint64_t beDecodeInt(const std::string *stringInt,
				             std::string::size_type startPosition,
				             std::string::size_type &endPosition);
	     
	    BeList* beDecodeList(const std::string *stringList, 
				             std::string::size_type startPosition,
				             std::string::size_type &endPosition);
	     
	    BeDict* beDecodeDict(const std::string *stringDict, 
				             std::string::size_type startPosition,
				             std::string::size_type &endPosition);	
     
		/**
		 * Devuelve el primer BeNode encontrado a partir de la posicion pasada
		 * por parametro en inicioNodo; a su vez devuelve la ultima posicion 
		 * leida de la cadena. En caso de leer el ultimo BeNode devuelve en
		 * finNodo BE_END.
		 * Para saber el tipo de dato que almacena el BeNode se debe leer el 
		 * typeNode; y luego, a partir de este, leer el dato correspondiente.
		 */
		BeNode* beDecode(const std::string *cadena,
		  	             std::string::size_type inicioNodo,
			             std::string::size_type &finNodo);

		/**
		 * Metodos utilizados por beFree para liberar cada nodo
		 */
		static void beFree(BeNode* beNode);
		static void beFree(BeDict* dict);
		static void beFree(BeList* list);

	public:
		/**
		 * Dado un archivo, parsea el contenido y lo devuelve en una lista 
      	 * de nodos.
         * 
         * @param nombreArchivo Nombre del archivo a parsear.
         * 
         * @return La lista de nodos resultado.
         */
     	std::list<BeNode*>* beDecode(const char* nombreArchivo);

		/**
		 * Dado un buffer, parsea el contenido y lo devuelve en una lista
	     * de nodos.
		 *
		 * @param buffer Buffer a parsear.
		 * 
		 * @return La lista de nodos resultado.
		 */
		std::list<BeNode*>* beDecode(const std::string& buffer);
		
		/**
		 * Libera la lista de nodos devuelta por beDecode.
		 *
		 * @param La lista de nodos a liberar.
		 */
		static void beFree(std::list<BeNode*>* nodeList);
};

/******************************************************************************/
#endif /*PARSERBENCODING_H_*/
