#include "GUIEventGenerator.h"
using namespace std;


genericEvent* GUIEventGenerator::
getEvent(unsigned int estado)
{
	genericEvent* ret = nullptr;
	switch (getGUIevent((implStates)estado))
	{
	case CrearNodoSPV:
		ret = new evCrearNodoSPV;
		break;

	case CrearNodoFULL:
		ret = new evCrearNodoFULL;
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
	switch(ev)
	{
		case GUIEvent::CrearNodoSPV:
			return implEvent::CrearNodoSPV;
		case GUIEvent::CrearNodoFULL:
			return implEvent::CrearNodoFULL;
		case GUIEvent::CrearConexion:
			return implEvent::CrearConexion;
		case GUIEvent::MostrarNodos:
			return implEvent::MostrarNodos;
		case GUIEvent::EnviarMsj:
			return implEvent::EnviarMsj;
		case GUIEvent::MostrarMsj:
			return implEvent::MostrarMsj;
		case GUIEvent::AccionDone:
			return implEvent::AccionDone;
		case GUIEvent::Quit:
			return implEvent::Quit;
		default: 
			return implEvent::NoEvent;
	}
}