#ifndef PROTOCOLOBITTORRENT_H_
#define PROTOCOLOBITTORRENT_H_

#include "../SHA1/bitStream.h"
#include <iostream>
#include <sstream>


class ProtocoloBitTorrent
{
	private:
			BitStream bitStream;
		
			std::string int64Astring(uint64_t valor){
				//convierto de int a string
				std::string snumero;
				std::stringstream cvz;
				cvz << valor;
				snumero = cvz.str();
				return snumero;
			
			}
			std::string int32Astring(uint32_t valor)
			{
				//convierto de int a string
				std::string snumero;
				std::stringstream cvz;
				cvz << valor;
				snumero = cvz.str();
				return snumero;
			}
	public:
			ProtocoloBitTorrent(){}
			
			std::string handshake(std::string str,std::string info_hash,std::string peer_id){
				char pstrlen = str.length();
				std::string mensajeHandshake;
				uint64_t reserved = 0;//Revisar, xq deberia mostrar los 8bytes de 0's
				mensajeHandshake = pstrlen + str + int64Astring(reserved) + info_hash + peer_id;
				//Opcion: que envie directamente el mensaje de a partes.
				return mensajeHandshake;
			}  
			
			uint32_t keep_alive(){
				uint32_t len = 0;
				len = this->bitStream.swap32ABigEndian(len);
				//Otra posibilidad, es que mande directamente el mensaje
				return len;
			}
			
			std::string choke(){
				uint32_t len = 1;
				len = this->bitStream.swap32ABigEndian(len);
				uint32_t id = 0;
				id = this->bitStream.swap32ABigEndian(id);
				//Opcion: enviar el mensaje desde aca sin devolver.
				return  int32Astring(len) + int32Astring(id);
			}
			
			std::string unchoke(){
				uint32_t len = 1;
				len = this->bitStream.swap32ABigEndian(len);
				uint32_t id = 1;
				id = this->bitStream.swap32ABigEndian(id);
				//Opcion: enviar el mensaje desde aca sin devolver.
				return  int32Astring(len) + int32Astring(id);
			}
			
			std::string interested(){
				uint32_t len = 1;
				len = this->bitStream.swap32ABigEndian(len);
				uint32_t id = 2;
				id = this->bitStream.swap32ABigEndian(id);
				//Opcion: enviar el mensaje desde aca sin devolver.
				return  int32Astring(len) + int32Astring(id);
			}
			
			std::string not_interested(){
				uint32_t len = 1;
				len = this->bitStream.swap32ABigEndian(len);
				uint32_t id = 3;
				id = this->bitStream.swap32ABigEndian(id);
				//Opcion: enviar el mensaje desde aca sin devolver.
				return  int32Astring(len) + int32Astring(id);
			}
			
			std::string have(){//ver si tiene que recibir el piece_index
				uint32_t len = 5;
				len = this->bitStream.swap32ABigEndian(len);
				uint32_t id = 4;
				id = this->bitStream.swap32ABigEndian(id);
				//TODO
				// ** VER PIECE INDEX **
				// aca va que parte fue bajada satisfactoriamente y verificada
				//con la informacion del hashing
				
				//Opcion: enviar el mensaje desde aca sin devolver.
				return  int32Astring(len) + int32Astring(id);
			}
			
			std::string bitfield(){//ver para recibir el bitfield de la parte que se descargo
				uint32_t len = 1;// + bitfield.length(); //la longitud de la parte
				len = this->bitStream.swap32ABigEndian(len);
				uint32_t id = 5;
				id = this->bitStream.swap32ABigEndian(id);
				//Opcion: enviar el mensaje desde aca sin devolver.
				return  int32Astring(len) + int32Astring(id);//+ bitfield;
			}
			
			
			~ProtocoloBitTorrent(){}	
};

#endif /*PROTOCOLOBITTORRENT_H_*/
