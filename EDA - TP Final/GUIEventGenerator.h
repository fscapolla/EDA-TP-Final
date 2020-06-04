

#ifndef GUIEVGENERATOR_H

#define GUIEVGENERATOR_H


#include "Structs.h"

#include "GraphicF2.h"

#include "eventHandling.h"

#include <array>
#include <vector>

/* EVENTOS FASE 2 */

enum implEvent : eventTypes { CrearNodo, CrearConexion, MostrarNodos, BuscarVecinos, EnviarMsj, Error, Back2Dashboard, NoEvent, BlockSelected, Quit };

//Heredan de clase genericEvent (EventHandling.h)
class evCrearNodo : public genericEvent
{
public:

	evCrearNodo(RegistroNodo_t registro_, std::vector<RegistroNodo_t>*vectorArrPtr,unsigned int ID_, string* nameofFile_)
	{
		PUERTO = registro_.PUERTO;
		IP = registro_.IP;
		TYPE = registro_.TYPE;
		NodoArray = vectorArrPtr;
		ID = ID_;
		nameofFile = nameofFile_;
	}

	eventTypes getType(void) { return CrearNodo; }		//Esto lo usan las rutinas de accion para verificar que se trata del evento correcto
	std::vector<RegistroNodo_t>* NodoArray;
	uint TYPE;
	uint PUERTO;
	string IP;
	unsigned int ID;
	string* nameofFile;
};

class evCrearConexion : public genericEvent
{
public:
	evCrearConexion(RegistroNodo_t nodo1_, RegistroNodo_t nodo2_, std::vector<RegistroNodo_t>* vectorArrPtr, string * nameofFile_) : nameofFile(nameofFile_), Nodo1(nodo1_), Nodo2(nodo2_), NodoArrayC(vectorArrPtr) {}

	eventTypes getType(void) { return CrearConexion; }
	
	RegistroNodo_t Nodo1;
	RegistroNodo_t Nodo2;
	std::vector<RegistroNodo_t>* NodoArrayC;
	string* nameofFile;
	
};

class evMostrarNodos : public genericEvent
{
public:
	eventTypes getType(void) { return MostrarNodos; }
	string IP2Connect; 

	evMostrarNodos(void) {}
	evMostrarNodos(string IP_) : IP2Connect(IP_) {}
};

class evBuscarVecinos : public genericEvent
{
public:
	eventTypes getType(void) { return BuscarVecinos; }

	string JSONPath;
	evBuscarVecinos(void) {}
	evBuscarVecinos(string JPATH_) : JSONPath(JPATH_){}
};

class evEnviarMsj : public genericEvent
{
public:
	evEnviarMsj(ParticipantesMsj_t comunicacion_, string* nameofFile_) : nameofFile(nameofFile_), Comunication(comunicacion_) {}

	eventTypes getType(void) { return EnviarMsj; }
	ParticipantesMsj_t Comunication;
	string* nameofFile;
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

class evBlockSelected : public genericEvent
{
public:
	eventTypes getType(void) { return BlockSelected; }
};

class evMulti : public genericEvent
{
public:
	eventTypes getType(void) { return NoEvent; }
};

//Hereda de clase eventGenerator (EventHandling.h)
class GUIEventGenerator : public eventGenerator
{
public:
	GUIEventGenerator(std::vector<SPVNode*>* SPVArrayPTR_, std::vector<FullNode*>* FULLArrayPTR_);

	std::vector<RegistroNodo_t>* getNodosArr(void);
	genericEvent* getEvent(unsigned int estadoactualdeFSM);
	bool getGraphicInstallationError();

private:
	/* PRINTING FUNCTIONS */
	GraphicF3* GUI;
	implEvent getGUIevent(unsigned int estado);
	implEvent TranslateGUIEvent(GUIEvent ev);
	unsigned int nodeID;
};

#endif //GUIEVGENE