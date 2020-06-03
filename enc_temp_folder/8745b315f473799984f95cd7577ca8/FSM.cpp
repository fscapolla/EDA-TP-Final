
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
		//	SPVNode tempSpv(static_cast<evCrearNodo*>(ev)->ID, static_cast<evCrearNodo*>(ev)->IP, static_cast<evCrearNodo*>(ev)->PUERTO);
		//	spvArray.push_back(tempSpv);
			input2file = "Se creo un nodo\nIP:" + static_cast<evCrearNodo*>(ev)->IP + " - ID: " + to_string(static_cast<evCrearNodo*>(ev)->ID) + " - TYPE: SPV \nPUERTO:" + to_string(static_cast<evCrearNodo*>(ev)->PUERTO) + "\n\n";
		}
		if (static_cast<evCrearNodo*>(ev)->TYPE == FULL)
		{
		//	FullNode tempFull(static_cast<evCrearNodo*>(ev)->ID, static_cast<evCrearNodo*>(ev)->IP, static_cast<evCrearNodo*>(ev)->PUERTO);
		//	fullArray.push_back(tempFull);
			input2file = "Se creo un nodo\nIP:" + static_cast<evCrearNodo*>(ev)->IP + " - ID: " + to_string(static_cast<evCrearNodo*>(ev)->ID) + " - TYPE: FULL\n" + "PUERTO:" + to_string(static_cast<evCrearNodo*>(ev)->PUERTO) + "\n\n";
		}

		RegistroNodo_t tempReg(static_cast<evCrearNodo*>(ev)->ID, static_cast<evCrearNodo*>(ev)->IP, static_cast<evCrearNodo*>(ev)->PUERTO, static_cast<evCrearNodo*>(ev)->TYPE);
			/***** ACA MANDAMOS UPDATE A BULLETIN   ******/
		cout << input2file << endl;

		* static_cast<evCrearNodo*>(ev)->nameofFile += input2file; 

		/****** ACTUALIZAMOS ARREGLO DE NODOS  *******/
		RegistroNodo_t tempNewNodo;
		tempNewNodo.ID = static_cast<evCrearNodo*>(ev)->ID;
		tempNewNodo.IP = static_cast<evCrearNodo*>(ev)->IP;
		tempNewNodo.TYPE = static_cast<evCrearNodo*>(ev)->TYPE;
		tempNewNodo.TYPE = static_cast<evCrearNodo*>(ev)->PUERTO;

		static_cast<evCrearNodo*>(ev)->NodoArray->push_back(tempNewNodo);

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

		/****** ACTUALIZAMOS ARREGLO DE NODOS  *******/
		vector<RegistroNodo_t> ptr = *static_cast<evCrearConexion*>(ev)->NodoArrayC;  /*  DESREFERENCIAMOS EL PUNTERO AL VECTOR DE NODOS */	

		// ---> NODO 1 CON NODO 2 COMO VECINO
		RegistroNodo_t tempNewNodo;
		Neighbour tempNei;

		tempNewNodo = static_cast<evCrearConexion*>(ev)->Nodo1 ;
	
		tempNei.IP = static_cast<evCrearConexion*>(ev)->Nodo2.IP;
		tempNei.port = static_cast<evCrearConexion*>(ev)->Nodo2.PUERTO;

		tempNewNodo.NodosVecinos.insert(pair<unsigned int, Neighbour>(static_cast<evCrearConexion*>(ev)->Nodo2.ID, tempNei));

		ptr[static_cast<evCrearConexion*>(ev)->Nodo1.ID] = tempNewNodo;


		// ---> NODO 2 CON NODO 1 COMO VECINO
		RegistroNodo_t tempNewNodo2;
		Neighbour tempNei2;

		tempNewNodo2 = static_cast<evCrearConexion*>(ev)->Nodo2;

		tempNei2.IP = static_cast<evCrearConexion*>(ev)->Nodo1.IP;
		tempNei2.port = static_cast<evCrearConexion*>(ev)->Nodo1.PUERTO;

		tempNewNodo2.NodosVecinos.insert(pair<unsigned int, Neighbour>(static_cast<evCrearConexion*>(ev)->Nodo1.ID, tempNei2));

		ptr[static_cast<evCrearConexion*>(ev)->Nodo2.ID] = tempNewNodo2;



		/***** ACA MANDAMOS UPDATE A BULLETIN   ******/
		string input2file;
		input2file = "Conexion creada entre\n Nodo: " + to_string(static_cast<evCrearConexion*>(ev)->Nodo1.ID) + "\n Nodo: " + to_string(static_cast<evCrearConexion*>(ev)->Nodo2.ID) + "\n\n";
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