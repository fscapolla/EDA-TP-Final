

#ifndef GUIEVGENERATOR_H

#define GUIEVGENERATOR_H

#include <array>

#include "Structs.h"
#include "GraphicF2.h"

#include "eventHandling.h"

/* EVENTOS FASE 2 */

enum implEvent : eventTypes { CrearNodo, CrearConexion, MostrarNodos, BuscarVecinos, EnviarMsj, Error, Back2Dashboard, NoEvent, Quit };

//Heredan de clase genericEvent (EventHandling.h)
class evCrearNodo : public genericEvent
{
public:

	evCrearNodo(RegistroNodo_t registro_, std::vector<RegistroNodo_t>*vectorArrPtr)
	{
		PUERTO = registro_.PUERTO;
		IP = registro_.IP;
		TYPE = registro_.TYPE;
		NodoArray = vectorArrPtr;
	}

	eventTypes getType(void) { return CrearNodo; }		//Esto lo usan las rutinas de accion para verificar que se trata del evento correcto
	std::vector<RegistroNodo_t>* NodoArray;
	uint TYPE;
	uint PUERTO;
	string IP;
};


class evCrearConexion : public genericEvent
{
public:
	evCrearConexion(RegistroNodo_t nodo1_, RegistroNodo_t nodo2_, std::vector<RegistroNodo_t>* vectorArrPtr) : Nodo1(nodo1_), Nodo2(nodo2_), NodoArrayC(vectorArrPtr) {}

	eventTypes getType(void) { return CrearConexion; }

	RegistroNodo_t Nodo1;
	RegistroNodo_t Nodo2;
	std::vector<RegistroNodo_t>* NodoArrayC;

};


class evMostrarNodos : public genericEvent
{
public:
	eventTypes getType(void) { return MostrarNodos; }
};

class evBuscarVecinos : public genericEvent
{
public:
	eventTypes getType(void) { return BuscarVecinos; }
};


class evEnviarMsj : public genericEvent
{
public:
	evEnviarMsj(ParticipantesMsj_t comunicacion_) : Comunication(comunicacion_) {}

	eventTypes getType(void) { return EnviarMsj; }
	ParticipantesMsj_t Comunication;
};

class evBack2Dashboard :public genericEvent
{
public:
	eventTypes getType(void) { return Back2Dashboard; }
};

class evQuit : public genericEvent
{
public:
	eventTypes getType(void) { return Quit; }
};

class evError : public genericEvent
{
public:
	eventTypes getType(void) { return Error; }
};




//Hereda de clase eventGenerator (EventHandling.h)
class GUIEventGenerator : public eventGenerator
{
public:

	GUIEventGenerator();

	std::vector<RegistroNodo_t>* getNodosArr(void);
	genericEvent* getEvent(unsigned int estadoactualdeFSM);
	bool getGraphicInstallationError();

private:
	/* PRINTING FUNCTIONS */
	GraphicF2 GUI;
	implEvent getGUIevent(unsigned int estado);
	implEvent TranslateGUIEvent(GUIEvent ev);

};

#endif //GUIEVGENE