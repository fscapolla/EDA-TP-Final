#ifndef STRUCTS_H
#define STRUCTS_H

#include<vector>
#include<string>
#include <stdio.h>
#include <map>

struct Neighbour2 {
	std::string IP;
	int port;
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
	std::map<unsigned int, Neighbour2> NodosVecinos;
	std::string mensaje;
	std::string vecinos;	//Esto se usa para la funcion combo de ImGui
	int selectedVecino;

} ParticipantesMsj_t;


/* EVENTOS FASE 2 */
//enum implEvent : eventTypes { CrearNodo, CrearConexion, MostrarNodos, BuscarVecinos, EnviarMsj, Error, Back2Dashboard, NoEvent, Quit };



enum class GUIEvent { CrearNodo, CrearConexion, MostrarNodos, BuscarVecinos, EnviarMsj, Error, Back2Dashboard, NoEvent, Quit };




#endif //STRUCTS_H