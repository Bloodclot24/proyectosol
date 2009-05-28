#ifndef PARSERBENCODING_H_
#define PARSERBENCODING_H_

#include <iostream>
#include <string>
#include <fstream>
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

typedef struct{
     std::list<BeNode*> elements;
}BeList;
		
typedef struct{
     std::map<std::string,BeNode*> elements;
}BeDict;

typedef struct BeNode {
	BeType typeNode;
	std::string beStr;
	long long beInt;
        BeList*	beList;
	BeDict*	beDict;
} BeNode;

/****************************************************************************/
class ParserBencode {
	
	private:
		std::string beDecodeStr(std::string stringStr, 
		                        std::string::size_type startPosition,
		                        std::string::size_type &endPosition);
	
		long long beDecodeInt(std::string stringInt,
				      std::string::size_type startPosition,
		                      std::string::size_type &endPosition);
		
		BeList* beDecodeList(std::string stringList, 
						std::string::size_type startPosition,
		                                std::string::size_type &endPosition);
		
		BeDict* beDecodeDict(std::string stringDict, 
						std::string::size_type startPosition,
						std::string::size_type &endPosition);	
	
	public:
		/*
		 * Devuelve el primer BeNode encontrado a partir de la posicion pasada
		 * por parametro en inicioNodo; a su vez devuelve la ultima posicion 
		 * leida de la cadena. En caso de leer el ultimo BeNode devuelve en
		 * finNodo BE_END.
		 * Para saber el tipo de dato que almacena el BeNode se debe leer el 
		 * typeNode; y luego, a partir de este, leer el dato correspondiente.
		 * */
		BeNode* beDecode(std::string cadena,
				 std::string::size_type inicioNodo,
		                 std::string::size_type &finNodo);          	
};

/****************************************************************************/
#endif /*PARSERBENCODING_H_*/
