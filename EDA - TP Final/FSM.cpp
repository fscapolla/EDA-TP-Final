#include "FSM.h"

void FSM::RutaDefault(genericEvent* ev)
{
	return;
}

void FSM::BuscarVecinos_r_acc(genericEvent* ev)
{
	if (static_cast<evBuscarVecinos*>(ev)->getType() == BuscarVecinos)
	{
		this->state4Graphic = LOOK4VECI_G;
		cout << "LOOK 4 VECINOS " << endl;
	}
}

void FSM::CrearNodo_r_acc(genericEvent* ev)
{
	if (static_cast<evCrearNodo*>(ev)->getType() == CrearNodo)
	{
		cout << "CREAR NODOS " << endl;
		/*******
			uint TYPE;
			uint PUERTO;
			string IP;
			std::vector<RegistroNodo_t>* NodoArray;
		*******/
		if (static_cast<evCrearNodo*>(ev)->TYPE == SPV)
		{
		//
		}
		if (static_cast<evCrearNodo*>(ev)->TYPE == FULL)
		{
		//
		}

		//ACA SI EDITAR VECTOR DE NODOS
	}
}

void FSM::VolverADashboard_r_acc(genericEvent* ev)
{
	if (static_cast<evBack2Dashboard*>(ev)->getType() == Back2Dashboard)
	{
		cout << " VOLVER A DASHBOARD " << endl;
		this->state4Graphic = DASHBOARD_G;
	}
}

void FSM::EnviarMensaje_r_acc(genericEvent* ev)
{
	if (static_cast<evBuscarVecinos*>(ev)->getType() == EnviarMsj)
	{
		cout << " ENVIAMOS MENSAJE " << endl;
		this->state4Graphic = DASHBOARD_G;
		/****************
		typedef struct
		{
			RegistroNodo_t NodoEmisor;
			std::map<unsigned int, Neighbour2> NodosVecinos;
			std::string mensaje;
			std::string vecinos;	//Esto se usa para la funcion combo de ImGui
			int selectedVecino;

		} ParticipantesMsj_t;

		ParticipantesMsj_t Comunication;
		
		std::vector<RegistroNodo_t>* NodoArray;
		*****************/

	}
}

void FSM::CrearConexion_r_acc(genericEvent* ev)
{

	if (static_cast<evBuscarVecinos*>(ev)->getType() == EnviarMsj)
	{
		cout << " CREAMOS CONEXION " << endl;
		/*
		RegistroNodo_t Nodo1;
		RegistroNodo_t Nodo2;

		typedef struct
		{
			std::string IP;
			int PUERTO;
			int ID;
			int TYPE;

		}RegistroNodo_t;
		
		std::vector<RegistroNodo_t>* NodoArrayC;
		*/
	}
}

void FSM::ShwNodos_r_acc(genericEvent* ev)
{
	if (static_cast<evBuscarVecinos*>(ev)->getType() == MostrarNodos)
	{
		cout << " MUESTRO NODOS " << endl;
		this->state4Graphic = SHWNODOS_G;
	}
}

void FSM::ErrorEncontrado_r_acc(genericEvent* ev)
{
	if (static_cast<evBuscarVecinos*>(ev)->getType() == Error)
	{
		cout << " ERROR ENCONTRADO " << endl;
		this->state4Graphic = SHWERROR_G;
	}
}