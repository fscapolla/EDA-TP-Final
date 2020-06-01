#include "GUIEventGenerator.h"
using namespace std;


genericEvent* GUIEventGenerator::
getEvent(unsigned int estado)
{
	genericEvent* ret = nullptr;
	switch (getGUIevent((implStates)estado))
	{
	case CrearNodo:
		ret = new evCrearNodo(this->GUI.getRegistro());
		break;

	case CrearConexion:
		ret = new evCrearConexion(this->GUI.getRegistro(), this->GUI.getRegistro());		//Toma los registos en cola
		break;

	case MostrarNodos:
		ret = new evMostrarNodos;
		break;

	case BuscarVecinos:
		ret = new evBuscarVecinos();
		break;

	case EnviarMsj:
		ret = new evEnviarMsj(this->GUI.getComunicacion());
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
		GUIEvent sendingEv = GUI.getEvent();
		return TranslateGUIEvent(sendingEv);
	}
	else
		return NoEvent;

}


bool GUIEventGenerator::getGraphicInstallationError()
{
	return GUI.GetError();
}

implEvent GUIEventGenerator::TranslateGUIEvent(GUIEvent ev)
{
	switch (ev)
	{
	case GUIEvent::CrearNodo:
		return implEvent::CrearNodo;

	case GUIEvent::CrearConexion:
		return implEvent::CrearConexion;

	case GUIEvent::MostrarNodos:
		return implEvent::MostrarNodos;

	case GUIEvent::EnviarMsj:
		return implEvent::EnviarMsj;

	case GUIEvent::AccionDone:
		return implEvent::AccionDone;

	case GUIEvent::Quit:
		return implEvent::Quit;

	case GUIEvent::Error:
		return implEvent::Error;

	default:
		return implEvent::NoEvent;
	}
}