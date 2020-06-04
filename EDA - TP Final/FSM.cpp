
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

		/***** ACTUALIZAMOS ARREGLO DE NODOS CREADOS EN ESTA INSTANCIA ******/
		RegistroNodo_t tempNewNodo;
		tempNewNodo.ID = static_cast<evCrearNodo*>(ev)->ID;
		tempNewNodo.TYPE = static_cast<evCrearNodo*>(ev)->TYPE;
		tempNewNodo.PUERTO = static_cast<evCrearNodo*>(ev)->PUERTO;

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

unsigned int FSM::getIndex(unsigned int senderID, nodeTypes nodeType)
{
	unsigned int index = -1;

	if (nodeType == FULL)
	{
		for (int i = 0; i < spvArray.size() && index == -1; i++)
		{
			if (fullArray[i]->getID() == senderID)
				index = i;
		}
		return index;
	}
	else
	{
		for (int i = 0; i < spvArray.size() && index == -1; i++)
		{
			if (spvArray[i]->getID() == senderID)
				index = i;
		}
		return index;
	}

}

unsigned int FSM::getneighbourIDfromPort(unsigned int neighbourPORT, nodeTypes nodetype)
{
	unsigned int neighbourID = -1;
	if (nodetype == FULL)
	{
		for (int i = 0; i < fullArray.size() && neighbourID == -1; i++)
		{
			if (fullArray[i]->getPort() == neighbourPORT)
				neighbourID = fullArray[i]->getID();
		}
		return neighbourID;
	}
	else
	{
		for (int i = 0; i < fullArray.size() && neighbourID == -1; i++)
		{
			if (fullArray[i]->getPort() == neighbourPORT)
				neighbourID = fullArray[i]->getID();
		}
		return neighbourID;
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
		/*
		switch ((static_cast<evEnviarMsj*>(ev)->Comunication.MENSAJE))
		{
		case FILTER_Genv:
			//Recupero el ID del vecino y el del sender
			int neighbourID = static_cast<evEnviarMsj*>(ev)->Comunication.selectedVecino;
			//Si se tiene el puerto del vecino pero no su ID puede hacerse int neighbourID=getneighbourIDfromPort(unsigned int neighbourPORT, nodeTypes neighbourType);
			unsigned int neighbourPort=(static_cast<evEnviarMsj*>(ev)->Comunication.NodosVecinosPT[neighbourID].port);
			unsigned int senderID=static_cast<evEnviarMsj*>(ev)->Comunication.NodoEmisor.ID;
			//Busco el índice en el arreglo de nodos SPV (sólo SPV pueden enviar mensajes tipo Filter).
			unsigned int senderIndex = getIndex(senderID,SPV);
			//Recupero la publickey del nodo y configuro para enviar el mensaje.
			spvArray[senderIndex]->POSTFilter(neighbourID, spvArray[senderIndex]->getKey());
			//¿Se puede forzar a que ocurra una vez el estado NOTHING acá? sino igual creo que no importa
			break;

		case GETBLOCKS_Genv:
			//Recupero el ID del vecino y el del sender
			int neighbourID = static_cast<evEnviarMsj*>(ev)->Comunication.selectedVecino;
			unsigned int neighbourPort = (static_cast<evEnviarMsj*>(ev)->Comunication.NodosVecinosPT[neighbourID].port);
			unsigned int senderID = static_cast<evEnviarMsj*>(ev)->Comunication.NodoEmisor.ID;
			//Busco el índice del nodo en el arreglo (sólo nodos full usan envían este mensaje)
			unsigned int senderIndex = getIndex(senderID,FULL);
			//Recupero valores de count y blockID (en esta fase no importan)
			//unsigned int count = 1;
			//std::string blockID = "75FF25E0";
			//Configuro el mensaje
				fullArray[senderIndex]->GETBlocks(neighbourID, (string) "75FF25E0", 1);
			break;

		case GETBLOCKHEADERS_Genv:
			//Recupero el ID del vecino y el del sender
			int neighbourID = static_cast<evEnviarMsj*>(ev)->Comunication.selectedVecino;
			unsigned int neighbourPort = (static_cast<evEnviarMsj*>(ev)->Comunication.NodosVecinosPT[neighbourID].port);
			unsigned int senderID = static_cast<evEnviarMsj*>(ev)->Comunication.NodoEmisor.ID;
			//Busco el índice del nodo en el arreglo (sólo nodos spv envían este mensaje)
			unsigned int senderIndex = getIndex(senderID, SPV);
			//Recupero valores de count y blockID (en esta fase no importan)
			//unsigned int count = 1;
			//std::string blockID = "75FF25E0";
			//Configuro el mensaje
			spvArray[senderIndex]->GETBlockHeader(neighbourID, (string) "75FF25E0", 1);
			break;

		case MERKLEBLOCK_Genv:
			//Recupero el ID del vecino y el del sender
			int neighbourID = static_cast<evEnviarMsj*>(ev)->Comunication.selectedVecino;
			unsigned int neighbourPort = (static_cast<evEnviarMsj*>(ev)->Comunication.NodosVecinosPT[neighbourID].port);
			unsigned int senderID = static_cast<evEnviarMsj*>(ev)->Comunication.NodoEmisor.ID;
			//Busco el índice del nodo en el arreglo (sólo nodos full envían este mensaje)
			unsigned int senderIndex = getIndex(senderID, FULL);
			//Recupero valores de BlockID y TxId (en esta fase no importan)
			//std::string TxID_="7B857A14"
			//std::string blockID = "75FF25E0";
			//Configuro el mensaje
			fullArray[senderIndex]->POSTMerkleBlock(neighbourID, (string) "75FF25E0", (string) "7B857A14");
			break;

		case BLOCK_Genv:
			//Recupero el ID del vecino y el del sender
			int neighbourID = static_cast<evEnviarMsj*>(ev)->Comunication.selectedVecino;
			unsigned int neighbourPort = (static_cast<evEnviarMsj*>(ev)->Comunication.NodosVecinosPT[neighbourID].port);
			unsigned int senderID = static_cast<evEnviarMsj*>(ev)->Comunication.NodoEmisor.ID;
			//Busco el índice del nodo en el arreglo (sólo nodos full envían este mensaje)
			unsigned int senderIndex = getIndex(senderID, FULL);
			//Recupero valor de BlockID (en esta fase no importa)
			//std::string BlockID="75FF25E0";
			fullArray[senderIndex]->POSTBlock(neighbourID, (string) "75FF25E0");
			break;

		case TRANSACTION_Genv:
			//Recupero el tipo de nodo
			nodeTypes type = (nodeTypes)static_cast<evEnviarMsj*>(ev)->Comunication.NodoEmisor.TYPE;
			//Recupero el ID del vecino y el del sender
			int neighbourID = static_cast<evEnviarMsj*>(ev)->Comunication.selectedVecino;
			unsigned int neighbourPort = (static_cast<evEnviarMsj*>(ev)->Comunication.NodosVecinosPT[neighbourID].port);
			unsigned int senderID = static_cast<evEnviarMsj*>(ev)->Comunication.NodoEmisor.ID;
			//Busco el índice del nodo en el arreglo
			if (type == FULL)
				unsigned int senderIndex = getIndex(senderID, FULL);
			else
				unsigned int senderIndex = getIndex(senderID, SPV);
			//Recupero el monto a enviar y la wallet a donde enviar y configuro el mensaje
			if (type == FULL)
				fullArray[senderIndex]->makeTransaction(neighbourID, static_cast<evEnviarMsj*>(ev)->Comunication.PublicKey_G, static_cast<evEnviarMsj*>(ev)->Comunication.COINS_G);
			else
				spvArray[senderIndex]->makeTransaction(neighbourID, static_cast<evEnviarMsj*>(ev)->Comunication.PublicKey_G, static_cast<evEnviarMsj*>(ev)->Comunication.COINS_G);
			break;

		default:
			break;
		}

		*/
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

void FSM::Start_genesis_r_acc(genericEvent* ev)
{
	/*
		string JSONPath;
	*/
	if (static_cast<evBuscarVecinos*>(ev)->getType() == BuscarVecinos)			//Usamos evento mostrar vecinos para no tener q crear evento nuevo 
	{
		cout << " START MODO GENESIS  " << static_cast<evBuscarVecinos*>(ev)->JSONPath << endl;
		this->state4Graphic = CREATING_CONNECTION_G;
	}

}


void FSM::Start_app_r_acc(genericEvent* ev)
{
	if (static_cast<evMostrarNodos*>(ev)->getType() == MostrarNodos)			//Usamos evento mostrar nodos para no tener q crear evento nuevo 
	{
		cout << " START MODO appendice  " << static_cast<evMostrarNodos*>(ev)->IP2Connect << endl;

		this->state4Graphic = DASHBOARD_G;
	}
}