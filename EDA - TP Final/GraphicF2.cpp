
#include "GraphicF2.h"


GraphicF3::GraphicF3(std::vector<SPVNode*>* SPVArrayPTR_, std::vector<FullNode*>* FULLArrayPTR_)
{
	if (AllegroInit() && ImguiInit())
	{
		al_register_event_source(this->queue, al_get_display_event_source(display));
		al_register_event_source(this->queue, al_get_mouse_event_source());
		al_register_event_source(this->queue, al_get_keyboard_event_source());

		window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
		window_flags |= ImGuiWindowFlags_NoMove;
		SPVArrayPTR = SPVArrayPTR_;
		FULLArrayPTR = FULLArrayPTR_;

		BulletinFileName = "C:/Users/inequ/source/repos/EDA-TP-FinalLPM/EDA - TP Final/MisNoticias.txt";
		MyHamachiIP = "localhost";
		//ESTO ES UNA TRUCHADA 
		//PERO ES POR AHORA, DESPS CUANDO CONECTEMOS OBTENEMOS BLOCKCHAIN DE CUALQUIER NODO FULL

		fs::path bPath("C:/Users/inequ/source/repos/EDA-TP-FinalLPM/EDA - TP Final");
		if (exists(bPath) && is_directory(bPath))
		{
			for (fs::directory_iterator iterator(bPath); iterator != fs::directory_iterator(); iterator++)
			{
				if (iterator->path().filename().string() == "blockChain32.json")
				{
					if (!pBchain.saveBlockInfo(iterator->path().filename().string())) {
						Error = false;
					}
					else
					{
						Error = true;
					}
				}
			}
		}
		else
		{
			Error = true;
		}
	}
	else
	{
		Error = true;
	}
}

GraphicF3::~GraphicF3()
{
	ImGui_ImplAllegro5_Shutdown();
	ImGui::DestroyContext();
	if (!Error)
	{
		al_shutdown_primitives_addon();
		al_uninstall_keyboard();
		al_uninstall_mouse();
		al_shutdown_image_addon();
	}
	if (queue)
		al_destroy_event_queue(queue);
	if (display)
		al_destroy_display(display);
}

bool GraphicF3::GetError()
{
	return Error;
}

bool GraphicF3::hayEvento(unsigned int EstadoActual)
{
	while (al_get_next_event(queue, &ev))
	{
		ImGui_ImplAllegro5_ProcessEvent(&ev);
	}

	if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
	{
		GUIQueue.push(GUIEvent::Quit);
	}

	print_current_state(EstadoActual);  //Devuelve true si hubo un evento (Usuario presiono un boton)
												//Todas las funciones de impresion BUSCAN eventos y las guardan en EventQueue			

	return !GUIQueue.empty();		//Si NO esta vacia hay evento
}

GUIEvent GraphicF3::getEvent()
{
	GUIEvent EventoParaEvGenerator = GUIQueue.front();
	GUIQueue.pop();
	return EventoParaEvGenerator;
}

void GraphicF3::print_current_state(unsigned int CurrentState)
{
	switch (CurrentState)
	{
	case  DASHBOARD_G:
		print_Dashboard();
		break;

	case LOOK4VECI_G:
		print_look4Veci();
		break;

	case SHWERROR_G:
		print_Error();
		break;

	case SHWNODOS_G:
		print_Nodos();
		break;

	case SHWSELB_G:
		print_SelBlockInfo();
		break;

	default:
		break;
	}
}

void GraphicF3::print_Dashboard()
{
	ImGui_ImplAllegro5_NewFrame();
	ImGui::NewFrame();

	/***********************************
		VENTANA: CREAR UN NODO
	************************************/
	ImGui::SetNextWindowPos(ImVec2(20, 10));
	ImGui::SetNextWindowSize(ImVec2(380, 180));
	ImGui::Begin(">> CREAR UN NODO <<", 0, window_flags);
	static char Puerto[MAX_PUERTO];
	static bool nodofull = false;
	static bool nodospv = false;
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Datos del nodo: ");

	ImGui::Checkbox("NODO FULL", &nodofull);
	ImGui::Checkbox("NODO SPV", &nodospv);
	ImGui::InputText("PUERTO DEL SERVIDOR:", Puerto, sizeof(char) * MAX_PUERTO);

	if (ImGui::Button(" >> CREAR NODO << ") && ((nodofull == true) || (nodospv == true)) && (sizeof(Puerto) != 0))
	{
		if (nodofull == true)
		{
			RegistroNodo_t tempRegistro;
			tempRegistro.IP = MyHamachiIP;
			tempRegistro.TYPE = FULL;
			tempRegistro.PUERTO = atoi(Puerto);
			registros.push(tempRegistro);
		}

		if (nodospv == true)
		{
			RegistroNodo_t tempRegistro;
			tempRegistro.IP = MyHamachiIP;
			tempRegistro.TYPE = SPV;
			tempRegistro.PUERTO = atoi(Puerto);
			registros.push(tempRegistro);
		}

		GUIQueue.push(GUIEvent::CrearNodo);

	}

	ImGui::End();


	/*************************************
	VENTANA: CREAR CONEXION ENTRE NODOS
	**************************************/

	ImGui::SetNextWindowPos(ImVec2(20, 200));
	ImGui::SetNextWindowSize(ImVec2(380, 200));
	ImGui::Begin(">> CREAR CONEXION ENTRE NODOS <<", 0, window_flags);

	static char NODO1[MAX_PUERTO];
	static char NODO2[MAX_PUERTO];

	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Puerto del nodo 1");

	ImGui::InputText("PUERTO 1", NODO1, sizeof(char) * MAX_PUERTO);

	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), " Puerto del nodo 2 ");

	ImGui::InputText("PUERTO 2", NODO2, sizeof(char) * MAX_PUERTO);


	if ((ImGui::Button(" >> CREAR << ")) && (verify((string)NODO1, (string)NODO2)))
	{
		RegistroNodo_t tempNodo1;
		tempNodo1.PUERTO = atoi(NODO1);
		RegistroNodo_t tempNodo2;
		tempNodo2.IP = atoi(NODO2);

		registros.push(tempNodo1);
		registros.push(tempNodo2);

		GUIQueue.push(GUIEvent::CrearConexion);
	}

	ImGui::End();

	/*************************************
	   VENTANA: ENVIAR MENSAJE
	**************************************/

	ImGui::SetNextWindowPos(ImVec2(20, 410));
	ImGui::SetNextWindowSize(ImVec2(380, 150));
	ImGui::Begin(">> ENVIAR MENSAJER A NODO <<", 0, window_flags);
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Inserte ID del nodo Emisor y su tipo (FULL o SPV)");

	static char emisor[MAX_ID];
	static bool type[2] = { false };

	ImGui::InputText("NODO EMISOR:", emisor, sizeof(char) * MAX_ID);
	ImGui::Checkbox("SPV:", &type[0]);
	ImGui::Checkbox("FULL:", &type[1]);


	if ((ImGui::Button(" >> BUSCAR VECINOS << ")) && (verify(atoi(emisor), type[0])))
	{
		GUIQueue.push(GUIEvent::BuscarVecinos);		//Se cambiara de estado en fsm para imprimir "Selecting Vecino"
	}
	ImGui::End();

	/*************************************
		VENTANA: BULLETIN
	**************************************/
	print_Bulletin();


	/*************************************
	VENTANA: MOSTRAR NODOS
	**************************************/

	ImGui::SetNextWindowPos(ImVec2(450, 10));
	ImGui::SetNextWindowSize(ImVec2(300, 100));
	ImGui::Begin(">> MOSTRAR NODOS CREADOS EN LA RED <<", 0, window_flags);

	if (ImGui::Button(" Presione aqui"))
	{
		GUIQueue.push(GUIEvent::MostrarNodos);
	}
	ImGui::End();


	/*************************************
		VENTANA: MOSTRAR BLOCK INFO
	**************************************/
	/* FILE CON BLOCKCHAIN SE CARGO EN CONSTRUCTOR ESTOTAMBN ES POR AHORA DESPS LO TOMAMOS DE UN NODOFULL*/

	ImGui::SetNextWindowPos(ImVec2(450, 200));
	ImGui::SetNextWindowSize(ImVec2(300, 400));
	ImGui::Begin(">> MOSTRAR BLOQUE <<", 0, window_flags);

	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Seleccione un bloque");

	int i;
	//Checkbox con imgui
	static bool checks[MAX_BLOCKS] = { false };
	unsigned char box = clickedBlock(checks, (pBchain.getBlocksArr()).size());
	for (i = 0; i < pBchain.getBlocksSize(); i++)
	{

		ImGui::Checkbox((pBchain.getBlocksArr())[i].getBlockID().c_str(), &checks[i]);
		if (numSelectedBlocks(checks, (pBchain.getBlocksArr()).size()) > 1)
			checks[box] = false;
	}
	ImGui::Text("");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Informacion que desea ver:");

	static bool Actions[4];		//4 acciones posibles

	ImGui::Checkbox("Show block information ", &Actions[SHOWINFO]);

	ImGui::Checkbox("Calculate Merkle Root", &Actions[CALCULATEMERKLE]);

	ImGui::Checkbox("Validate Merkle Root", &Actions[VALIDATEMERKLE]);

	ImGui::Checkbox("Show Merkle Tree", &Actions[SHOWMERKLE]);

	if (ImGui::Button(" Select All "))
	{
		int i;
		for (i = 0; i < 4; i++)
			Actions[i] = true;
	}

	if (ImGui::Button(" >> GET RESULTS << "))
	{

		bool AlgunoSelected = false;
		for (i = 0; i < (pBchain.getBlocksArr()).size(); i++) {
			if (checks[i] == true)
			{
				selectedBlock.push_back((pBchain.getBlocksArr())[i]);
				i = pBchain.getBlocksArr().size();		//Solo selecciono uno
				AlgunoSelected = true;
			}
		}
		if (AlgunoSelected)
		{
			if (Actions[CALCULATEMERKLE] || Actions[SHOWMERKLE])
			{
				//Calculamos merkle tree y guardamos si es valido o no
				ValidationMerkleRoot = selectedBlock[0].createMerkleTree();
			}
			int i;
			for (i = 0; i < 4; i++)
				ActionsArray[i] = Actions[i];

			GUIQueue.push(GUIEvent::BlockSelected);
		}
		else
			GUIQueue.push(GUIEvent::Back2Dashboard);
	}

	ImGui::End();

	/*************************************
		VENTANA: ACCIONES
	**************************************/
	print_Acciones();

	/*************************************************************************************************
			FIN IMPRESION VENTANAS
	**************************************************************************************************/
	//Rendering
	ImGui::Render();

	al_clear_to_color(COLOR);

	ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
	al_flip_display();
}

bool GraphicF3::look4BlocksPath(string ChosenFile)
{
	fs::path bPath(directoryName);
	if (exists(bPath) && is_directory(bPath))
	{
		for (fs::directory_iterator iterator(bPath); iterator != fs::directory_iterator(); iterator++)
		{
			if (iterator->path().filename().string() == ChosenFile.c_str())
			{
				std::cout << iterator->path().string() << std::endl;
				if (!pBchain.saveBlockInfo(iterator->path().filename().string())) {
					return true;
				}
				else {
					GUIQueue.push(GUIEvent::Error);
					return false;
				}
			}
		}
	}
	else
	{
		GUIQueue.push(GUIEvent::Error);
		return false;
	}
}

void GraphicF3::print_Bulletin(void)		//IMPORTANTE se llama depsues de haber creado un NewFrame en otra parte del programa
{

	ImGui::SetNextWindowPos(ImVec2(800, 10));
	ImGui::SetNextWindowSize(ImVec2(550, 600));
	ImGui::Begin(">>    BULLETIN BOARD   <<", 0, window_flags);

	//	std::vector<SPVNode*>* SPVArrayPTR;
	//std::vector<SPVNode*>* FULLArrayPTR;


	cout <<  (*SPVArrayPTR).size() << endl;
	BulletinFile.open(BulletinFileName.c_str(), ios::out | ios::binary);		//archivo donde guardare info de nodos

	if (BulletinFile.is_open())		//verifico denuevo porlas
	{
		BulletinFile << this->BulletinText.c_str() << endl;
		std::ifstream in(BulletinFileName.c_str());
		std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), contents.c_str());

		BulletinFile.close();
	}


	ImGui::End();
}

void GraphicF3::print_look4Veci(void)
{
	ImGui_ImplAllegro5_NewFrame();
	ImGui::NewFrame();

	/*************************************
	   VENTANA: BULLETIN
	**************************************/
	print_Bulletin();

	/**************************************
		 VENTANA: ELEJIR VECINO + MSJ
	***************************************/
	ImGui::SetNextWindowPos(ImVec2(20, 10));
	ImGui::SetNextWindowSize(ImVec2(400, 550));
	ImGui::Begin(">> COMUNICACION ENTRE NODOS <<", 0, window_flags);

	ParticipantesMsj_t ComEnProgreso = Comunicaciones.front();


	ImGui::Text(" ");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), " >> Seleccione un vecino << ");

	//Checkbox con imgui
	int i;
	static int selectedN = -1;
	for (i = 0; i < ComEnProgreso.vecinos.size(); i++)
	{
		char buf[MAX_VECINOS];
		sprintf(buf, ComEnProgreso.vecinos[i].c_str());
		if (ImGui::Selectable(buf, selectedN == i))
			selectedN = i;
	}

	ImGui::Text(" ");


	static int selected = -1;
	static char CantCoins[10000];
	static char PKey[40];

	switch (ComEnProgreso.NodoEmisor.TYPE)
	{
	case FULL:
		char buf[32];
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), " >> RECIBIR << ");

		//ImGui::Text(">> RECIBIR <<");
		if (ImGui::TreeNode("Operaciones para recibir:"))
		{
			sprintf(buf, "Get blocks");
			{
				if (ImGui::Selectable(buf, selected == GETBLOCKS_Grec))
					selected = GETBLOCKS_Grec;
			}
			sprintf(buf, "Get block headers");
			{
				if (ImGui::Selectable(buf, selected == GETBLOCKHEADERS_Grec))
					selected = GETBLOCKHEADERS_Grec;
			}
			sprintf(buf, "Block");
			{
				if (ImGui::Selectable(buf, selected == BLOCK_Grec))
					selected = BLOCK_Grec;
			}
			sprintf(buf, "Filter");
			{
				if (ImGui::Selectable(buf, selected == FILTER_Grec))
					selected = FILTER_Grec;
			}
			ImGui::TreePop();
		}

		ImGui::Text(" ");
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), " >> ENVIAR << ");

		if (ImGui::TreeNode("Operaciones para enviar:"))
		{
			if (ImGui::TreeNode("Datos para transaccion"))
			{
				ImGui::InputText("CANTIDAD:", CantCoins, IM_ARRAYSIZE(CantCoins));

				ImGui::InputText("PUBLIC KEY:", PKey, IM_ARRAYSIZE(PKey));

				sprintf(buf, "Transaccion");
				{
					if (ImGui::Selectable(buf, selected == GETBLOCKS_Genv))
						selected = GETBLOCKS_Genv;
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Otras operaciones"))
			{
				sprintf(buf, "Get blocks");
				{
					if (ImGui::Selectable(buf, selected == GETBLOCKS_Genv))
						selected = GETBLOCKS_Genv;
				}
				sprintf(buf, "Block");
				{
					if (ImGui::Selectable(buf, selected == BLOCK_Genv))
						selected = BLOCK_Genv;
				}
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		break;

	case SPV:

		char bufSPV[32];
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), " >> RECIBIR << ");

		if (ImGui::TreeNode("Operaciones para recibir:"))
		{
			sprintf(bufSPV, "MerkleBlock");
			{
				if (ImGui::Selectable(bufSPV, selected == MERKLEBLOCK_Grec))
					selected = MERKLEBLOCK_Grec;
			}
			ImGui::TreePop();
		}

		ImGui::Text(" ");
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), " >> ENVIAR << ");


		if (ImGui::TreeNode("Operaciones para enviar:"))
		{
			if (ImGui::TreeNode("Datos para transaccion"))
			{
				ImGui::InputText("CANTIDAD:", CantCoins, IM_ARRAYSIZE(CantCoins));


				ImGui::InputText("PUBLIC KEY:", PKey, IM_ARRAYSIZE(PKey));

				sprintf(bufSPV, "Transaccion");
				{
					if (ImGui::Selectable(bufSPV, selected == TRANSACTION_Genv))
						selected = TRANSACTION_Genv;
				}
				ImGui::TreePop();
			}

			sprintf(bufSPV, "Get block headers");
			{
				if (ImGui::Selectable(bufSPV, selected == GETBLOCKHEADERS_Genv))
					selected = GETBLOCKHEADERS_Genv;
			}

			sprintf(bufSPV, "Filter");
			{
				if (ImGui::Selectable(bufSPV, selected == FILTER_Genv))
					selected = FILTER_Genv;
			}

			ImGui::TreePop();
		}
		break;
	}

	ImGui::Text(" ");
	ImGui::Text(" ");

	if (ImGui::Button(" >> ENVIAR MENSAJE << ") && verify(selected, CantCoins, PKey, selectedN))
	{
		Comunicaciones.front().MENSAJE = selected;
		Comunicaciones.front().selectedVecino = selectedN;
		if (selected == TRANSACTION_Genv)
		{
			Comunicaciones.front().COINS_G = atoi(CantCoins);
			Comunicaciones.front().PublicKey_G = PKey;
		}
		GUIQueue.push(GUIEvent::EnviarMsj);

	}

	ImGui::End();

	/*************************************
		VENTANA: ACCIONES
	**************************************/
	print_Acciones();


	//Rendering
	ImGui::Render();

	al_clear_to_color(COLOR);

	ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
	al_flip_display();

}

void GraphicF3::print_Nodos()
{
	ImGui_ImplAllegro5_NewFrame();
	ImGui::NewFrame();

	/****************************************
		VENTANA: MOSTRAMOS NODOS CREADOS
	*****************************************/
	ImGui::SetNextWindowPos(ImVec2(20, 10));
	ImGui::SetNextWindowSize(ImVec2(600, 600));
	ImGui::Begin(">> NODOS CREADOS EN LA RED <<", 0, window_flags);
	static bool h_borders = true;
	static bool v_borders = true;

	int columns_count = 5;
	const int lines_count = 5;
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);

	ImGui::Columns(columns_count, NULL, v_borders);
	for (int i = 0; i < NodosArray.size(); i++)
	{
		if (h_borders && ImGui::GetColumnIndex() == 0)
			ImGui::Separator();
		ImGui::Text("ID: %i \nIP: %s\n", NodosArray[i].ID, NodosArray[i].IP.c_str());
		ImGui::NextColumn();
	}
	
	ImGui::End();


	print_Acciones();
	/*************************************************************************************************
			FIN IMPRESION VENTANAS
	**************************************************************************************************/
	//Rendering
	ImGui::Render();

	al_clear_to_color(COLOR);

	ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
	al_flip_display();
}

unsigned char GraphicF3::clickedBlock(bool* checks, size_t size)
{

	int i;
	unsigned char count = 0;
	for (i = 0; i < size; i++) {
		if (*(checks + i))
			return count;
		count++;
	}
	return count;
}

unsigned char GraphicF3::numSelectedBlocks(bool* checks, size_t size)
{

	int i;
	unsigned char count = 0;
	for (i = 0; i < size; i++) {
		if (*(checks + i))
			count++;
	}
	return count;
}

void GraphicF3::print_SelBlockInfo(void) {
	ImGui_ImplAllegro5_NewFrame();
	ImGui::NewFrame();

	if (ActionsArray[SHOWINFO])
	{
		ImGui::SetNextWindowPos(ImVec2(50, 10));
		ImGui::SetNextWindowSize(ImVec2(350, 150));
		ImGui::Begin(" INFORMACION DEL BLOQUE ");
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Block ID: ");
		ImGui::SameLine();
		ImGui::Text("%s", selectedBlock[0].getBlockID().c_str());
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Previous Block ID: ");
		ImGui::SameLine();
		ImGui::Text("%s", selectedBlock[0].getPrevBlovkID().c_str());
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Cantidad de transacciones: ");
		ImGui::SameLine();
		ImGui::Text("%u", selectedBlock[0].getNtx());
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Numero de bloque: ");
		ImGui::SameLine();
		ImGui::Text("%u", selectedBlock[0].getHeight());
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Nounce: ");
		ImGui::SameLine();
		ImGui::Text("%u", selectedBlock[0].getNonce());
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Merkle Root: ");
		ImGui::SameLine();
		ImGui::Text("%s", selectedBlock[0].getMerkleRoot().c_str());
		ImGui::End();

	}

	if (ActionsArray[CALCULATEMERKLE])
	{
		ImGui::SetNextWindowPos(ImVec2(420, 10));
		ImGui::SetNextWindowSize(ImVec2(230, 70));
		ImGui::Begin(" CALCULO DE MERKLE ROOT");

		ImGui::Text("Merkel Root: %s", selectedBlock[0].getCalculatedMerkleRoot().c_str());		//Se calculó merkleRoot al presionar boton start
		ImGui::End();
	}

	if (ActionsArray[VALIDATEMERKLE])
	{
		ImGui::SetNextWindowPos(ImVec2(670, 10));
		ImGui::SetNextWindowSize(ImVec2(200, 70));
		ImGui::Begin(" VALIDACION DE MERKLE ROOT");

		if (ValidationMerkleRoot)
		{
			ImGui::Text(" MERKLE ROOT IS");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), " VALID");

		}
		else
		{
			ImGui::Text(" MERKLE ROOT IS");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "NOT VALID");
		}
		ImGui::End();
	}

	if (ActionsArray[SHOWMERKLE])
	{
		//printLevel(uint altura, MerkleTree tree) Imprimos recursivamente
		//Empezamos imprimiendo todas las hojas de la base	
		printLevel(0, selectedBlock[0].getNumLeaves(), selectedBlock[0].getMerkleHeight(), selectedBlock[0].getNodos());
	}

	print_Acciones();

	ImGui::Render();

	al_clear_to_color(COLOR);

	if (ActionsArray[SHOWMERKLE])		//Imprimios las conexiones entre nodos del Merkle Tree con allegro
	{
		int i, j;
		uint Nodos = selectedBlock[0].getNumLeaves();

		for (i = 0; i <= (selectedBlock[0].getMerkleHeight()); i++)
		{
			for (j = 0; j <= Nodos; j++)		//Empiezo imprimiendo las hojas
			{
				if (Nodos != 1)
				{
					drawConnections(i, Nodos);
				}
			}
			Nodos = Nodos / 2;
		}

	}

	ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());

	al_flip_display();

}

void GraphicF3::printLevel(uint altura, uint NodosAImprimir, uint TreeHeight, vector<string>Nodos)
{
	uint i;
	int increase_X = 0, levelIncrease = 0, increase_Y = LEVEL_INCREASE_Y * altura;
	increase_X = starterValue(altura, "increaseX");
	levelIncrease = starterValue(altura, "levelInc");

	char level = 'A' + altura;

	if (altura != TreeHeight + 1)		//Cuando lleguemos a altura 5 dejamos de imprimir. Altura empieza en 0
	{
		for (i = 0; i < NodosAImprimir; i++)		//Imprimimos una fila de hojas (empezamos por nivel de abajo y vamos subiendo)
		{
			string nodolabel = level + to_string(i + 1);
			ImGui::SetNextWindowPos(ImVec2(INITIAL_X + increase_X, INITIAL_Y - increase_Y));
			ImGui::Begin(nodolabel.c_str(), 0, window_flags);
			ImGui::Text("%s", Nodos[i].c_str());
			ImGui::End();
			increase_X += levelIncrease;
		}

		vector<string> NodosSiguienteNivel = vector<string>(Nodos.begin() + NodosAImprimir, Nodos.end());
		printLevel(++altura, NodosAImprimir / 2, TreeHeight, NodosSiguienteNivel);
	}
}

void GraphicF3::drawConnections(int altura, uint Nodos) {

	al_set_target_backbuffer(display);
	int i;														//falta corregir los pubtos de conexion,creo que sumar medio bloque alcanza
	int lowerNodeX = INITIAL_X + starterValue(altura, "increaseX")
		, lowerNodeY = INITIAL_Y - LEVEL_INCREASE_Y * altura
		, upperNodeX = INITIAL_X + starterValue(altura + 1, "increaseX")
		, upperNodeY = INITIAL_Y - LEVEL_INCREASE_Y * (altura + 1);

	int lnodeIncrease = starterValue(altura, "levelIncrease"),
		unodeIncrease = starterValue(altura + 1, "levelIncrease");

	for (i = 0; i < Nodos; i++) {

		al_draw_line(lowerNodeX, lowerNodeY, upperNodeX, upperNodeY, al_color_name("black"), 1.0);
		lowerNodeX += lnodeIncrease;
		if (i % 2) {
			upperNodeX += unodeIncrease;
		}

	}

}

int GraphicF3::starterValue(uint altura, const char* var) {

	int increase_x = 0, levelIncrease = 0;
	switch (altura) {
	case 0:
		increase_x = 0;
		levelIncrease = LEVEL_INCREASE_X;
		break;
	case 1:
		increase_x = LEVEL_INCREASE_X / 2;
		levelIncrease = LEVEL_INCREASE_X * 2;
		break;
	case 2:
		increase_x = LEVEL_INCREASE_X * 3 / 2;
		levelIncrease = LEVEL_INCREASE_X * 4;
		break;
	case 3:
		increase_x = LEVEL_INCREASE_X * 7 / 2;
		levelIncrease = LEVEL_INCREASE_X * 8;
		break;
	case 4:
		increase_x = LEVEL_INCREASE_X * 15 / 2;
		levelIncrease = LEVEL_INCREASE_X * 16;
		break;
	case 5:
		increase_x = LEVEL_INCREASE_X * 31 / 2;
		levelIncrease = LEVEL_INCREASE_X * 32;
		break;
	}

	if (var == "increaseX")
		return increase_x;
	else
		return levelIncrease;

}

void GraphicF3::print_Acciones()		//Importante llamarla entre newframe y render
{

	ImGui::SetNextWindowPos(ImVec2(1400, 10));
	ImGui::SetNextWindowSize(ImVec2(250, 100));
	ImGui::Begin(">> ACCIONES <<", 0, window_flags);

	if (ImGui::Button(" >> Quit << "))
	{
		GUIQueue.push(GUIEvent::Quit);
	}

	if (ImGui::Button(" >>  VOLVER A DASHBOARD  << "))
	{
		GUIQueue.push(GUIEvent::Back2Dashboard);
		cout << "VOLVER  A DASHBOARD BUTTON " << endl;
	}

	ImGui::End();
}

ParticipantesMsj_t GraphicF3::getComunicacion(void)
{
	ParticipantesMsj_t Comunicacion = Comunicaciones.front();		//Sabemos que hay una comunicacion en la cola porque lo verifico verify()
	Comunicaciones.pop();

	return Comunicacion;
}

/********************************
	VERIFY PARA ENVIAR MENSAJE
*********************************/
bool GraphicF3::verify(unsigned int tipo, string coinss, string pkeyyyy, int alguienFueSeleccionado)
{
	if (((tipo == TRANSACTION_Genv) && (coinss.empty() || pkeyyyy.empty())) || (alguienFueSeleccionado == -1))
	{
		return false;
	}
	else
		return true;
}

bool GraphicF3::verify(uint ExisteEsteNodo, bool esUnNodoSPV)
{
	bool ret = false;

	ParticipantesMsj_t tempParticipantes;

	if (esUnNodoSPV)		//Si es SPV esto es TRUE
	{
		tempParticipantes.NodoEmisor.TYPE = SPV;
	}
	else
	{
		tempParticipantes.NodoEmisor.TYPE = FULL;
	}
	for (auto itnodo : NodosArray)
	{
		if (itnodo.ID == ExisteEsteNodo)		//Si encontramos ese ID entonces existe nodo proseguimos a buscar sus vecinos
		{
			tempParticipantes.NodoEmisor.ID = itnodo.ID;
			tempParticipantes.NodoEmisor.IP = itnodo.IP;
			tempParticipantes.NodoEmisor.PUERTO = itnodo.PUERTO;

			tempParticipantes.NodosVecinosPT = itnodo.NodosVecinos;
			string tempIDVecino;
			for (auto vecinos : itnodo.NodosVecinos)
			{
				tempIDVecino.append("IP: " + vecinos.second.IP + " - PORT: " + to_string(vecinos.second.port) + "\0");
			}
			tempIDVecino.append("\0");

			this->Comunicaciones.push(tempParticipantes);
			ret = true;
		}
	}
	if (ret == false)
	{
		GUIQueue.push(GUIEvent::Error);
	}


	return true;
}

bool GraphicF3::verify(string nodo1, string nodo2)
{
	if ((!nodo1.empty() && !nodo2.empty()))
	{
		return true;
	}
	else
		return false;
}

void GraphicF3::print_Error(void)
{
	ImGui_ImplAllegro5_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(400, 270));
	ImGui::SetNextWindowSize(ImVec2(300, 300));

	ImGui::Begin("No se puede hacer esa operacion", 0, window_flags);

	if (ImGui::Button(" >> Quit << "))
	{
		GUIQueue.push(GUIEvent::Quit);
	}

	if (ImGui::Button(" >>  VOLVER A DASHBOARD  << "))
	{
		GUIQueue.push(GUIEvent::Back2Dashboard);
	}

	ImGui::End();

	ImGui::Render();

	al_clear_to_color(al_map_rgb(179, 0, 0));

	ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
	al_flip_display();
}

RegistroNodo_t GraphicF3::getRegistro(void)
{
	RegistroNodo_t returning = registros.front();

	registros.pop();

	return returning;
}

bool GraphicF3::AllegroInit()
{
	if (al_init())
	{
		if (al_init_image_addon())
		{
			if (al_install_mouse())
			{
				if (al_install_keyboard())
				{
					if (al_init_primitives_addon())
					{
						if (this->queue = al_create_event_queue()) {

							return true;
						}
						else {

							printf("ERROR al_init_primitives_addon");
							al_uninstall_keyboard();
							al_shutdown_image_addon();
							al_uninstall_mouse();
							al_destroy_event_queue(this->queue);
						}

					}
					else
					{
						printf("ERROR al_init_primitives_addon");
						al_uninstall_keyboard();
						al_shutdown_image_addon();
						al_uninstall_mouse();
					}
				}
				else
				{
					printf("ERROR al_instal_keyboard\n");
					al_shutdown_image_addon();
					al_uninstall_mouse();
				}
			}
			else
			{
				printf("ERROR al_install_mouse\n");
				al_shutdown_image_addon();
			}
		}
		else
			printf("ERROR al_init_image_addon\n");
	}
	else
		printf("ERROR al_init\n");
	return false;
}

bool GraphicF3::ImguiInit(void)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	display = al_create_display(SIZE_SCREEN_X, SIZE_SCREEN_Y);
	if (display)
	{
		al_set_window_position(display, 0, 100); //posicion del menu
		al_acknowledge_resize(display);
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		ImGui_ImplAllegro5_Init(display);
	}
	else
		return false;

	return true;
}

std::vector<RegistroNodo_t>* GraphicF3::getNodoArray(void) {

	return &NodosArray;
}