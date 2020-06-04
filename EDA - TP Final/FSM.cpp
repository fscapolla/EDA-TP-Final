
#include "FSM.h"

FSM::~FSM()
{
	for (auto& node : spvArray) {
		if (node)
			delete node;
	}
	for (auto& node : fullArray) {
		if (node)
			delete node;
	}
}

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

std::vector<SPVNode*>* FSM::getSPVArrayPTR(void)
{
	return &spvArray;
}
std::vector<FullNode*>* FSM::getFULLArrayPTR(void)
{
	return &fullArray;
}

void FSM::CrearNodo_r_acc(genericEvent* ev)
{
	if (static_cast<evCrearNodo*>(ev)->getType() == CrearNodo)
	{
		/*******
			uint TYPE;
			int PUERTO;
			string IP;
			std::vector<RegistroNodo_t>* NodoArray;
			int nodeID;
			std::string * nameofFile;
		*******/

		string input2file;

		if (static_cast<evCrearNodo*>(ev)->TYPE == SPV)
		{
			SPVNode * TempSPVNode = new SPVNode(static_cast<evCrearNodo*>(ev)->ID, static_cast<evCrearNodo*>(ev)->IP, static_cast<evCrearNodo*>(ev)->PUERTO);
			
			spvArray.push_back(TempSPVNode);
			input2file = "Se creo un nodo\nIP:" + static_cast<evCrearNodo*>(ev)->IP + " - ID: " + to_string(static_cast<evCrearNodo*>(ev)->ID) + " - TYPE: SPV \nPUERTO:" + to_string(static_cast<evCrearNodo*>(ev)->PUERTO) + "\n\n";
		}
		if (static_cast<evCrearNodo*>(ev)->TYPE == FULL)
		{
			FullNode * tempFullNode = new FullNode(static_cast<evCrearNodo*>(ev)->ID, static_cast<evCrearNodo*>(ev)->IP, static_cast<evCrearNodo*>(ev)->PUERTO);
			
			fullArray.push_back(tempFullNode);

			input2file = "Se creo un nodo\nIP:" + static_cast<evCrearNodo*>(ev)->IP + " - ID: " + to_string(static_cast<evCrearNodo*>(ev)->ID) + " - TYPE: FULL\n" + "PUERTO:" + to_string(static_cast<evCrearNodo*>(ev)->PUERTO) + "\n\n";
		}

		RegistroNodo_t tempReg(static_cast<evCrearNodo*>(ev)->ID, static_cast<evCrearNodo*>(ev)->IP, static_cast<evCrearNodo*>(ev)->PUERTO, static_cast<evCrearNodo*>(ev)->TYPE);
			/***** ACA MANDAMOS UPDATE A BULLETIN   ******/

		*static_cast<evCrearNodo*>(ev)->nameofFile += input2file; 

		/****** ACTUALIZAMOS ARREGLO DE NODOS  *******/
		RegistroNodo_t tempNewNodo;
		tempNewNodo.ID = static_cast<evCrearNodo*>(ev)->ID;
		tempNewNodo.IP = static_cast<evCrearNodo*>(ev)->IP;
		tempNewNodo.TYPE = static_cast<evCrearNodo*>(ev)->TYPE;
		tempNewNodo.TYPE = static_cast<evCrearNodo*>(ev)->PUERTO;

		static_cast<evCrearNodo*>(ev)->NodoArray->push_back(tempNewNodo);

	}
}

void FSM::MultiiPerform(genericEvent* ev)
{
	for (const auto& spvnode : spvArray)
		spvnode->performRequest();

	for (const auto& fullnode : fullArray)
		fullnode->performRequest();
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
	if (static_cast<evEnviarMsj*>(ev)->getType() == EnviarMsj)
	{
		this->state4Graphic = DASHBOARD_G;
		/****************
		typedef struct
		{
			RegistroNodo_t NodoEmisor;
			std::map<unsigned int, Neighbour> NodosVecinos;
			std::string mensaje;
			std::string vecinos;	//Esto se usa para la funcion combo de ImGui
			int selectedVecino;

		} ParticipantesMsj_t;

		ParticipantesMsj_t Comunication;
		
		std::vector<RegistroNodo_t>* NodoArray;
		*****************/

		Neighbour NodoReceptor = static_cast<evEnviarMsj*>(ev)->Comunication.NodosVecinosPT[static_cast<evEnviarMsj*>(ev)->Comunication.selectedVecino];



		/***** ACA MANDAMOS UPDATE A BULLETIN   ******/
		string input2file;
		input2file = "Se envió un mensaje\n Emisor: " + to_string(static_cast<evEnviarMsj*>(ev)->Comunication.NodoEmisor.ID) + "\n Receptor\n   IP: " + NodoReceptor.IP + "    PUERTO:" + to_string(NodoReceptor.port) + "\n\n";
		cout << input2file << endl;

		*static_cast<evEnviarMsj*>(ev)->nameofFile += input2file;
	}
}

void FSM::CrearConexion_r_acc(genericEvent* ev)
{

	if (static_cast<evCrearConexion*>(ev)->getType() == CrearConexion)
	{
		cout << " CREAMOS CONEXION " << endl;

		/*
			
		RegistroNodo_t Nodo1;
		RegistroNodo_t Nodo2;
		std::vector<RegistroNodo_t>* NodoArrayC;
		string* nameofFile;
		
		*/


		/***** ACA MANDAMOS UPDATE A BULLETIN   ******/
		string input2file;
		cout << static_cast<evCrearConexion*>(ev)->Nodo1.IP << endl;
		input2file = "Conexion creada entre\n Nodo: " + static_cast<evCrearConexion*>(ev)->Nodo1.IP + "\n Nodo: " + static_cast<evCrearConexion*>(ev)->Nodo2.IP + "\n\n";
		cout << input2file << endl;

		*static_cast<evCrearConexion*>(ev)->nameofFile += input2file;

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

void FSM::BlockSelected_r_acc(genericEvent* ev)
{
	if (static_cast<evBlockSelected*>(ev)->getType() == BlockSelected)
	{
		this->state4Graphic = SHWSELB_G;
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