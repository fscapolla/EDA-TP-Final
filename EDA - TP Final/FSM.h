#pragma once
#include "GenericFSM.h"
#define TX(x) (static_cast<void (genericFSM::* )(genericEvent *)>(&FSM::x)) //casteo a funcion, por visual


enum implStates : stateTypes { MostrandoMenu, ShowingError, Loading, State0 , State1, State2, State3 };

using namespace std;
class FSM : public genericFSM
{

public:
	FSM() : genericFSM(&fsmTable[0][0], 4, 6, State0) {}

private:
	const fsmCell fsmTable[4][6] = {
//EVENTOS:     Crear Nodo                         Crear Conexion                  Mostrar Nodos                  Enviar Mensaje              Mostrar Estado             Accion Done
		{  	{State0,TX(RutaDefault)},	       	{State1,TX(RutaDefault)},		{State2,TX(RutaDefault)},		{State3,TX(RutaDefault)},    {State3,TX(RutaDefault)} , { State3,TX(RutaDefault) }},        //Mostrando Menu
		{	{State1,TX(RutaDefault)},		    {State2,TX(RutaDefault)},		{State3,TX(RutaDefault)},		{State0,TX(RutaDefault)},    {State3,TX(RutaDefault)},  {State3, TX(RutaDefault) }},        //State1
		{	{State2,TX(RutaDefault)},		    {State3,TX(RutaDefault)},		{State0,TX(RutaDefault)},		{State1,TX(RutaDefault)},    {State3,TX(RutaDefault)},  {State3, TX(RutaDefault) }},        //State2
		{	{State3,TX(RutaDefault)},		    {State0,TX(RutaDefault)},		{State1,TX(RutaDefault)},		{State2,TX(RutaDefault)},    {State3,TX(RutaDefault)},  {State3,TX(RutaDefault) }}          //State3 
	};

	//The action routines for the FSM
	//These actions should not generate fsmEvents

	/* FUNCIONES */
	//En gui: Que las funciones editen campos a imprimir
	//En Networking: Que funciones hagan las acciones

	void RutaDefault(genericEvent* ev);
};