#pragma once
#include "GenericFSM.h"
#include "GUIEventGenerator.h"

#define TX(x) (static_cast<void (genericFSM::* )(genericEvent *)>(&FSM::x)) //casteo a funcion, por visual


enum implStates : stateTypes { ShwDashboard, Look4Veci, ShwNodos, ShwError };

using namespace std;
class FSM : public genericFSM
{

public:
	FSM() : genericFSM(&fsmTable[0][0], 4, 7, ShwDashboard) {}

private:
	const fsmCell fsmTable[4][7] = {
		//EVENTOS:		Crear Nodo							 Crear Conexion							Mostrar Nodos                      Buscar Vecinos                   Back2Dashboard                      Error								No event
//ESTADOS 
		/*Shw Dashboard*/		{{ShwDashboard,TX(RutaDefault)},     {ShwDashboard,TX(RutaDefault)},		{ShwDashboard,TX(RutaDefault)},		{Look4Veci,TX(RutaDefault)},     {ShwDashboard,TX(RutaDefault)},     {ShwError,TX(RutaDefault)} ,      {ShwDashboard,TX(RutaDefault) }},

		/*  Look4Veci  */		{{Look4Veci,TX(RutaDefault)},		 {Look4Veci,TX(RutaDefault)},			{Look4Veci,TX(RutaDefault)},		{Look4Veci,TX(RutaDefault)},     {ShwDashboard,TX(RutaDefault)},     {ShwError,TX(RutaDefault)},       {Look4Veci, TX(RutaDefault) }},

		/*  ShwNodos   */		{{ShwNodos,TX(RutaDefault)},	     {ShwNodos,TX(RutaDefault)},			{ShwNodos,TX(RutaDefault)},			{ShwNodos,TX(RutaDefault)},		 {ShwDashboard,TX(RutaDefault)},     {ShwError,TX(RutaDefault)},		{ShwNodos, TX(RutaDefault) }},

		/*   ShwError  */		{{ShwError,TX(RutaDefault)},		 {ShwError,TX(RutaDefault)},			{ShwError,TX(RutaDefault)},			{ShwError,TX(RutaDefault)},      {ShwDashboard,TX(RutaDefault)},	 {ShwError,TX(RutaDefault)},		{ShwError,TX(RutaDefault) }}

	};

	//The action routines for the FSM
	//These actions should not generate fsmEvents

	/* FUNCIONES */


	void RutaDefault(genericEvent* ev);
	void BuscamosVecinos(genericEvent* ev);
};