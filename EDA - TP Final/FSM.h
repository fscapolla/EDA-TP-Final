#pragma once
#include "GenericFSM.h"
#include "GUIEventGenerator.h"
#include <vector>

#define TX(x) (static_cast<void (genericFSM::* )(genericEvent *)>(&FSM::x)) //casteo a funcion, por visual


enum implStates : stateTypes { ShwDashboard, Look4Veci, ShwNodos, ShwSelBlock , ShwError };

using namespace std;
class FSM : public genericFSM
{

public:
	FSM() : genericFSM(&fsmTable[0][0], 5, 9, ShwDashboard), state4Graphic(DASHBOARD_G) {}
	unsigned int state4Graphic;

private:
	const fsmCell fsmTable[5][9] = {
		//enum implEvent : eventTypes { CrearNodo, CrearConexion, MostrarNodos, BuscarVecinos, EnviarMsj, Error,Back2Dashboard,  NoEvent, Quit };

				//EVENTOS:		          Crear Nodo							 Crear Conexion									Mostrar Nodos						Buscar Vecinos								EnviarMsj									Error										Back2Dashboard										No event							BlockSelected
		//ESTADOS 
		/*Shw Dashboard*/		{{ShwDashboard,TX(CrearNodo_r_acc)},     {ShwDashboard,TX(CrearConexion_r_acc)},		{ShwNodos,TX(ShwNodos_r_acc)},		{Look4Veci,TX(BuscarVecinos_r_acc)},		{ShwDashboard,TX(RutaDefault)},				{ShwError,TX(ErrorEncontrado_r_acc)} ,				{ShwDashboard,TX(VolverADashboard_r_acc)} ,			{ShwDashboard,TX(RutaDefault)} ,	{ShwSelBlock,TX(BlockSelected_r_acc) }},

		/*  Look4Veci  */		{{Look4Veci,TX(RutaDefault)},			{Look4Veci,TX(RutaDefault)},					{Look4Veci,TX(RutaDefault)},		{Look4Veci,TX(RutaDefault)},				{ShwDashboard,TX(EnviarMensaje_r_acc)},     {ShwError,TX(ErrorEncontrado_r_acc)},				{ShwDashboard,TX(VolverADashboard_r_acc)} ,			{Look4Veci, TX(RutaDefault) } ,		{ShwDashboard,TX(RutaDefault) }},

		/*  ShwNodos   */		{{ShwNodos,TX(RutaDefault)},			{ShwNodos,TX(RutaDefault)},						{ShwNodos,TX(RutaDefault)},			{ShwNodos,TX(RutaDefault)},					{ShwDashboard,TX(RutaDefault)},				{ShwError,TX(ErrorEncontrado_r_acc)},				{ShwDashboard,TX(VolverADashboard_r_acc)}	,		{ShwNodos, TX(RutaDefault)} ,		{ShwDashboard,TX(RutaDefault) }},

	/*  ShwSelectedBlock */		{{ShwNodos,TX(RutaDefault)},			{ShwNodos,TX(RutaDefault)},						{ShwNodos,TX(RutaDefault)},			{ShwNodos,TX(RutaDefault)},					{ShwDashboard,TX(RutaDefault)},				{ShwError,TX(ErrorEncontrado_r_acc)},				{ShwDashboard,TX(VolverADashboard_r_acc)}	,		{ShwNodos, TX(RutaDefault)} ,		{ShwDashboard,TX(RutaDefault) }},
		
	/*   ShwError  */			{{ShwError,TX(RutaDefault)},			{ShwError,TX(RutaDefault)},						{ShwError,TX(RutaDefault)},			{ShwError,TX(RutaDefault)},					{ShwDashboard,TX(RutaDefault)},				{ShwError,TX(ErrorEncontrado_r_acc)},				{ShwDashboard,TX(VolverADashboard_r_acc)} , 		{ShwError,TX(RutaDefault)} ,		{ShwDashboard,TX(RutaDefault) }}

	};

	//The action routines for the FSM
	//These actions should not generate fsmEvents

	/* FUNCIONES */

	std::vector<SPVNode> spvArray;
	std::vector<FullNode> fullArray;
	std::ofstream BulletinFileR_ACC;

	void RutaDefault(genericEvent* ev);
	void BuscarVecinos_r_acc(genericEvent* ev);
	void EnviarMensaje_r_acc(genericEvent* ev);
	void CrearNodo_r_acc(genericEvent* ev);
	void CrearConexion_r_acc(genericEvent* ev);
	void VolverADashboard_r_acc(genericEvent* ev);
	void ErrorEncontrado_r_acc(genericEvent* ev);
	void ShwNodos_r_acc(genericEvent* ev);
	void BlockSelected_r_acc(genericEvent* ev);
};