

#include "GUIEventGenerator.h"

using namespace std;

GUIEventGenerator::GUIEventGenerator(std::vector<SPVNode*>* SPVArrayPTR_, std::vector<FullNode*>* FULLArrayPTR_)
{
	GUI = new GraphicF3(SPVArrayPTR_,FULLArrayPTR_);

	nodeID = 0;
}

genericEvent* GUIEventGenerator::
getEvent(unsigned int estado)
{
	genericEvent* ret = nullptr;
	switch (getGUIevent(estado))
	{
	case implEvent::CrearNodo:
		ret = new evCrearNodo(this->GUI->getRegistro(), this->GUI->getNodoArray(), this->nodeID, &(this->GUI->BulletinText));
		this->nodeID++;
		break;

	case implEvent::CrearConexion:
		ret = new evCrearConexion(this->GUI->getRegistro(), this->GUI->getRegistro(), this->GUI->getNodoArray() , &(this->GUI->BulletinText));		//Toma los registos en cola
		break;

	case implEvent::MostrarNodos:
		ret = new evMostrarNodos(this->GUI->MyHamachiIP);
		break;

	case implEvent::BuscarVecinos:
		ret = new evBuscarVecinos(this->GUI->BlockJSONFile);
		break;

	case implEvent::EnviarMsj:
		ret = new evEnviarMsj(this->GUI->getComunicacion(), &(this->GUI->BulletinText));
		break;

	case implEvent::Back2Dashboard:
		ret = new evBack2Dashboard;
		break;
		
	case implEvent::BlockSelected:
		ret = new evBlockSelected;
		break;

	case implEvent::Error:
		ret = new evError;
		break;

	case implEvent::Quit:
		ret = new evQuit;
		break;

	case implEvent::NoEvent:
		ret = new evMulti;
		break;
	}

	return ret;
}

implEvent GUIEventGenerator::getGUIevent(unsigned int estadoActual)
{
	implEvent sendingEvent = implEvent::NoEvent;

	if (GUI->hayEvento(estadoActual))
	{
		GUIEvent sendingEv = GUI->getEvent();
		sendingEvent = TranslateGUIEvent(sendingEv);
	}


	return sendingEvent;
}


bool GUIEventGenerator::getGraphicInstallationError()
{
	return GUI->GetError();
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

	case GUIEvent::BlockSelected:
		returning = implEvent::BlockSelected;
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
		break;

	case GUIEvent::Quit:
		returning = implEvent::Quit;
		break;

	default:
		break;
	}

	return returning;
}

std::vector<RegistroNodo_t>* GUIEventGenerator::getNodosArr(void) {

	return GUI->getNodoArray();

}