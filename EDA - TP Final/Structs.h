#ifndef STRUCTS_H
#define STRUCTS_H

#include<vector>
#include<string>
#include <stdio.h>
#include <map>

enum MSJESRec { BLOCK_Grec, TRANSACTION_Grec, MERKLEBLOCK_Grec, FILTER_Grec, GETBLOCKHEADERS_Grec, GETBLOCKS_Grec };
enum MSJESEnv { BLOCK_Genv, TRANSACTION_Genv, MERKLEBLOCK_Genv, FILTER_Genv, GETBLOCKHEADERS_Genv, GETBLOCKS_Genv };

struct Neighbour {
	std::string IP;
	unsigned int port;
};

typedef struct
{
	std::string IP;
	int PUERTO;
	int ID;
	int TYPE;

}RegistroNodo_t;



typedef struct
{
	RegistroNodo_t NodoEmisor;
	std::map<unsigned int, Neighbour> NodosVecinos;
	std::string mensaje;
	std::vector<std::string> vecinos;	//Esto se usa para la funcion combo de ImGui
	int selectedVecino;


	unsigned int MENSAJE;		//Tipo de mensaje 
	unsigned int COINS_G;
	std::string PublicKey_G;


} ParticipantesMsj_t;


/* EVENTOS FASE 2 */
//enum implEvent : eventTypes { CrearNodo, CrearConexion, MostrarNodos, BuscarVecinos, EnviarMsj, Error, Back2Dashboard, NoEvent, Quit };



enum class GUIEvent { CrearNodo, CrearConexion, MostrarNodos, BuscarVecinos, EnviarMsj, Error, Back2Dashboard, NoEvent, Quit };




#endif //STRUCTS_H