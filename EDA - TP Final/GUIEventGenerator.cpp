#include "GUIEventGenerator.h"
using namespace std;


genericEvent* GUIEventGenerator::
getEvent(unsigned int estado)
{
	genericEvent* ret = nullptr;
	switch (getGUIevent(estado))
	{
	case implEvent::CrearNodo:
		ret = new evCrearNodo(this->GUI.getRegistro());
		break;

	case implEvent::CrearConexion:
		ret = new evCrearConexion(this->GUI.getRegistro(), this->GUI.getRegistro());		//Toma los registos en cola
		break;

	case implEvent::MostrarNodos:
		ret = new evMostrarNodos;
		break;

	case implEvent::BuscarVecinos:
		ret = new evBuscarVecinos();
		break;

	case implEvent::EnviarMsj:
		ret = new evEnviarMsj(this->GUI.getComunicacion());
		break;

	case implEvent::Quit:
		ret = new evQuit;
		break;
	}

	return ret;
}

implEvent GUIEventGenerator::getGUIevent(unsigned int estadoActual)
{
	implEvent sendingEvent = NoEvent;
	if (GUI.hayEvento(estadoActual))
	{
		GUIEvent sendingEv = GUI.getEvent();
		sendingEvent = TranslateGUIEvent(sendingEv);
	}


	return sendingEvent;
}


bool GUIEventGenerator::getGraphicInstallationError()
{
	return GUI.GetError();
}

implEvent GUIEventGenerator::TranslateGUIEvent(GUIEvent ev)
{
	implEvent returning = NoEvent;
	/*
		enum implEvent : eventTypes { CrearNodo, CrearConexion, MostrarNodos, BuscarVecinos, EnviarMsj, Error, Back2Dashboard, NoEvent, Quit };
	*/

	switch (ev)
	{
	case GUIEvent::CrearNodo:
		returning = implEvent::CrearNodo;
		break;

	case GUIEvent::CrearConexion:
		returning = implEvent::CrearConexion;
		break;

	case GUIEvent::MostrarNodos:
		returning = implEvent::MostrarNodos;
		break;

	case GUIEvent::BuscarVecinos:
		returning = implEvent::BuscarVecinos;
		break;

	case GUIEvent::EnviarMsj:
		returning = implEvent::EnviarMsj;
		break;

	case GUIEvent::Error:
		returning = implEvent::Error;
		break;

	case GUIEvent::Back2Dashboard:
		returning = implEvent::Back2Dashboard;

	case GUIEvent::Quit:
		returning = implEvent::Quit;
		break;

	default:
		break;
	}

	return returning;
}