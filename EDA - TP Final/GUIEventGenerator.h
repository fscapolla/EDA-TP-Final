#pragma once
#pragma once
#include <array>

#include "eventHandling.h"
#include "FSM.h"		//La incluyo para tener los ImplStates
#include "GraphicF2.h"


/* EVENTOS FASE 2 */
enum implEvent : eventTypes {  CrearNodo, CrearConexion, MostrarNodos, BuscarVecinos, EnviarMsj, MostrarMsj, AccionDone, Quit, NoEvent, CrearNodoSPV, CrearNodoFULL,};

//Heredan de clase genericEvent (EventHandling.h)
class evCrearNodoSPV : public genericEvent
{
public:
	eventTypes getType(void) { return CrearNodoSPV; }
};

class evCrearNodoFULL : public genericEvent
{
public:
	eventTypes getType(void) { return CrearNodoFULL; }
};

class evCrearConexion : public genericEvent
{
public:
	eventTypes getType(void) { return CrearConexion; }
};

class evMostrarNodos : public genericEvent
{
public:
	eventTypes getType(void) { return MostrarNodos; }
};

class evBuscarVecinos: public genericEvent
{
public:
	eventTypes getType(void) { return BuscarVecinos; }
};

class evEnviarMsj : public genericEvent
{
public:
	eventTypes getType(void) { return EnviarMsj; }
};

class evMostrarMsj : public genericEvent
{
public:
	eventTypes getType(void) { return MostrarMsj; }
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
