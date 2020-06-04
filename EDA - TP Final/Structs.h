#ifndef STRUCTS_H
#define STRUCTS_H

#include<vector>
#include<string>
#include <stdio.h>
#include <map>

enum MSJESRec { BLOCK_Grec, TRANSACTION_Grec, MERKLEBLOCK_Grec, FILTER_Grec, GETBLOCKHEADERS_Grec, GETBLOCKS_Grec, BLOCK_Genv, TRANSACTION_Genv, MERKLEBLOCK_Genv, FILTER_Genv, GETBLOCKHEADERS_Genv, GETBLOCKS_Genv };
//typedef enum {SPV,FULL} nodeTypes;

struct Neighbour {
	std::string IP;
	int port;
};

typedef struct RegistroNodo_t
{
	std::string IP;
	int PUERTO;
	int ID;
	unsigned int TYPE;
	std::map<unsigned int, Neighbour> NodosVecinos;

	RegistroNodo_t(int ID_, std::string IP_, int PUERTO_, int TYPE_) :ID(ID_), IP(IP_), PUERTO(PUERTO_), TYPE(TYPE_) {}
	RegistroNodo_t() {}

}RegistroNodo_t;




typedef struct
{
	RegistroNodo_t NodoEmisor;
	std::map<unsigned int, Neighbour> NodosVecinosPT;
	std::string mensaje;
	std::vector<std::string> vecinos;	//Esto se usa para la funcion combo de ImGui
	int selectedVecino;


	unsigned int MENSAJE;		//Tipo de mensaje 
	unsigned int COINS_G;
	std::string PublicKey_G;


} ParticipantesMsj_t;


/* EVENTOS FASE 2 */
//enum implEvent : eventTypes { CrearNodo, CrearConexion, MostrarNodos, BuscarVecinos, EnviarMsj, Error, Back2Dashboard, NoEvent, Quit };



enum class GUIEvent { CrearNodo, CrearConexion, MostrarNodos, BuscarVecinos, EnviarMsj, Error, Back2Dashboard, NoEvent, BlockSelected , Quit };




#endif //STRUCTS_H