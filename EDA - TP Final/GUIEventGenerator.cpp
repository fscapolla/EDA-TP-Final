#include "GUIEventGenerator.h"
using namespace std;


genericEvent* GUIEventGenerator::
getEvent(unsigned int estado)
{
	genericEvent* ret = nullptr;
	switch (getGUIevent((implStates)estado))
	{
	case CrearNodo:
		ret = new evCrearNodo;
		break;
	
	case CrearConexion:
		ret = new evCrearConexion;
		break;

	case MostrarNodos:
		ret = new evMostrarNodos;
		break;

	case EnviarMsj:
		ret = new evEnviarMsj;
		break;

	case MostrarMsj:
		ret = new evMostrarMsj;
		break;

	case AccionDone:
		ret = new evAccionDone;
		break;

	case Quit:
		ret = new evQuit;
		break;
	}
	return ret;
}

implEvent GUIEventGenerator::getGUIevent(implStates estadoActual)
{
	if (GUI.hayEvento(estadoActual))
	{
		return GUI.getEvent();
	}
	else
		return NoEvent;

}


bool GUIEventGenerator::getGraphicInstallationError()
{
	return GUI.GetError();
}