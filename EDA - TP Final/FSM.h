#pragma once
#include "GenericFSM.h"
#include "GUIEventGenerator.h"
#include <vector>

#define TX(x) (static_cast<void (genericFSM::* )(genericEvent *)>(&FSM::x)) //casteo a funcion, por visual


enum implStates : stateTypes {InitState, ShwDashboard, Look4Veci, ShwNodos, ShwSelBlock , ShwError };

using namespace std;
class FSM : public genericFSM
{
public:
	FSM() : genericFSM(&fsmTable[0][0], 6, 9, InitState), state4Graphic(INITSTATE_G) {}
	~FSM();

	unsigned int state4Graphic;
	std::vector<SPVNode*>* getSPVArrayPTR(void);
	std::vector<FullNode*>* getFULLArrayPTR(void);


private:
	const fsmCell fsmTable[6][9] = {
		//enum implEvent : eventTypes { CrearNodo, CrearConexion, MostrarNodos, BuscarVecinos, EnviarMsj, Error,Back2Dashboard,  NoEvent, Quit };

				//EVENTOS:		          Crear Nodo							 Crear Conexion									Mostrar Nodos						Buscar Vecinos								EnviarMsj									Error										Back2Dashboard										No event							BlockSelected
		//ESTADOS																										/*Start app mode en init state*/		/*SAtart genesis mode en init state*/
		/* Init State */		{{InitState,TX(RutaDefault)},			{InitState,TX(CrearConexion_r_acc)},			{ShwDashboard,TX(Start_app_r_acc)},	{InitState,TX(Start_genesis_r_acc)},		{ShwDashboard,TX(RutaDefault)},				{ShwError,TX(ErrorEncontrado_r_acc)} ,				{ShwDashboard,TX(VolverADashboard_r_acc)} ,			{InitState,TX(RutaDefault)} ,	{ShwSelBlock,TX(BlockSelected_r_acc) }},

		/*Shw Dashboard*/		{{ShwDashboard,TX(CrearNodo_r_acc)},     {ShwDashboard,TX(CrearConexion_r_acc)},		{ShwNodos,TX(ShwNodos_r_acc)},			{Look4Veci,TX(BuscarVecinos_r_acc)},		{ShwDashboard,TX(RutaDefault)},				{ShwError,TX(ErrorEncontrado_r_acc)} ,				{ShwDashboard,TX(VolverADashboard_r_acc)} ,			{ShwDashboard,TX(MultiiPerform)} ,	{ShwSelBlock,TX(BlockSelected_r_acc) }},

		/*  Look4Veci  */		{{Look4Veci,TX(RutaDefault)},			{Look4Veci,TX(RutaDefault)},					{Look4Veci,TX(RutaDefault)},			{Look4Veci,TX(RutaDefault)},				{ShwDashboard,TX(EnviarMensaje_r_acc)},     {ShwError,TX(ErrorEncontrado_r_acc)},				{ShwDashboard,TX(VolverADashboard_r_acc)} ,			{Look4Veci, TX(MultiiPerform) } ,		{ShwDashboard,TX(RutaDefault) }},

		/*  ShwNodos   */		{{ShwNodos,TX(RutaDefault)},			{ShwNodos,TX(RutaDefault)},						{ShwNodos,TX(RutaDefault)},				{ShwNodos,TX(RutaDefault)},					{ShwDashboard,TX(RutaDefault)},				{ShwError,TX(ErrorEncontrado_r_acc)},				{ShwDashboard,TX(VolverADashboard_r_acc)}	,		{ShwNodos, TX(MultiiPerform)} ,		{ShwDashboard,TX(RutaDefault) }},

	/*  ShwSelectedBlock */		{{ShwNodos,TX(RutaDefault)},			{ShwNodos,TX(RutaDefault)},						{ShwNodos,TX(RutaDefault)},				{ShwNodos,TX(RutaDefault)},					{ShwDashboard,TX(RutaDefault)},				{ShwError,TX(ErrorEncontrado_r_acc)},				{ShwDashboard,TX(VolverADashboard_r_acc)}	,		{ShwSelBlock, TX(MultiiPerform)} ,		{ShwDashboard,TX(RutaDefault) }},
		
	/*   ShwError  */			{{ShwError,TX(RutaDefault)},			{ShwError,TX(RutaDefault)},						{ShwError,TX(RutaDefault)},				{ShwError,TX(RutaDefault)},					{ShwDashboard,TX(RutaDefault)},				{ShwError,TX(ErrorEncontrado_r_acc)},				{ShwDashboard,TX(VolverADashboard_r_acc)} , 		{ShwError,TX(MultiiPerform)} ,		{ShwDashboard,TX(RutaDefault) }}

	};

	//The action routines for the FSM
	//These actions should not generate fsmEvents

	/* FUNCIONES */

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
	void MultiiPerform(genericEvent* ev);
	void Start_genesis_r_acc(genericEvent* ev);
	void Start_app_r_acc(genericEvent* ev);
	unsigned int getIndex(unsigned int senderID, nodeTypes nodeType);
	unsigned int getneighbourIDfromPort(unsigned int neighbourPORT, nodeTypes nodetype);

	std::vector<SPVNode*> spvArray;
	std::vector<FullNode*> fullArray;

//	void SaveNode(SPVNode& spvNode);
//	void SaveNode(FullNode& fullNode);

};