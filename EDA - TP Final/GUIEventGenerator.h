
#ifndef GUIEVGENERATOR_H
#define GUIEVGENERATOR_H

#include <array>

#include "Structs.h"
#include "GraphicF2.h"

#include "eventHandling.h"

/* EVENTOS FASE 2 */

enum implEvent : eventTypes { CrearNodo, CrearConexion, MostrarNodos, BuscarVecinos, EnviarMsj, MostrarMsj, AccionDone, Quit, NoEvent, Error };

//Heredan de clase genericEvent (EventHandling.h)
class evCrearNodo : public genericEvent
{
public:
	evCrearNodo(RegistroNodo_t registro_)
	{
		PUERTO = registro_.PUERTO;
		IP = registro_.IP;
		TYPE = registro_.TYPE;
	}

	eventTypes getType(void) { return CrearNodo; }		//Esto lo usan las rutinas de accion para verificar que se trata del evento correcto

	uint TYPE;
	uint PUERTO;
	string IP;
};


class evCrearConexion : public genericEvent
{
public:
	evCrearConexion(RegistroNodo_t nodo1_, RegistroNodo_t nodo2_) : Nodo1(nodo1_), Nodo2(nodo2_) {}

	eventTypes getType(void) { return CrearConexion; }

	RegistroNodo_t Nodo1;
	RegistroNodo_t Nodo2;
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


class evAccionDone : public genericEvent
{
public:
	eventTypes getType(void) { return AccionDone; }
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
	//	GUIEventGenerator();
	genericEvent* getEvent(unsigned int estadoactualdeFSM);
	bool getGraphicInstallationError();

private:
	/* PRINTING FUNCTIONS */
	implEvent getGUIevent(implStates estado);
	GraphicF2 GUI;
	implEvent TranslateGUIEvent(GUIEvent ev);

};

#endif //GUIEVGENERATOR_H