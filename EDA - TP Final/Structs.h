#ifndef STRUCTS_H
#define STRUCTS_H

#include<vector>
#include<string>
#include <stdio.h>
#include <map>

struct Neighbour2 {
	std::string IP;
	unsigned int port;
};

typedef struct
{
	/*	RegistroNodo_t(std::string ip_, uint puerto_, uint type_) : IP(ip_), PUERTO(puerto_), TYPE(type_) {}
		RegistroNodo_t(uint id_, uint type_) : ID(id_), TYPE(type_) {}
		RegistroNodo_t(uint id2_) : ID(id2_) {}
		RegistroNodo_t() {}
	*/

	//	void setParticipante(std::string ip_, uint puerto_, uint id_) { IP = ip_; PUERTO = puerto_; ID = id_; }

	std::string IP;
	unsigned int PUERTO;
	unsigned int ID;
	unsigned int TYPE;

}RegistroNodo_t;

typedef struct
{
	RegistroNodo_t NodoEmisor;
	std::map<unsigned int, Neighbour2> NodosVecinos;
	std::string mensaje;
	std::string vecinos;	//Esto se usa para la funcion combo de ImGui
	int selectedVecino;

} ParticipantesMsj_t;





#endif //STRUCTS_H