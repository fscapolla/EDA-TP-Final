

#include "GraphicF2.h"

GraphicF2::GraphicF2()
{
	if (AllegroInit() && ImguiInit())
	{
		al_register_event_source(this->queue, al_get_display_event_source(display));
		al_register_event_source(this->queue, al_get_mouse_event_source());
		al_register_event_source(this->queue, al_get_keyboard_event_source());

		window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
		window_flags |= ImGuiWindowFlags_NoMove;


	}
	else
	{
		Error = true;
	}
}

GraphicF2::~GraphicF2()
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

bool GraphicF2::GetError()
{
	return Error;
}

bool GraphicF2::hayEvento(unsigned int EstadoActual)
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

GUIEvent GraphicF2::getEvent()
{
	GUIEvent EventoParaEvGenerator = GUIQueue.front();
	GUIQueue.pop();
	return EventoParaEvGenerator;
}

void GraphicF2::print_current_state(unsigned int CurrentState)
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
		//print_Nodos();

	default:
		break;
	}
}

void GraphicF2::print_Dashboard()
{
	ImGui_ImplAllegro5_NewFrame();
	ImGui::NewFrame();

	/***********************************
		VENTANA: CREAR UN NODO
	************************************/
	ImGui::SetNextWindowPos(ImVec2(20, 10));
	ImGui::SetNextWindowSize(ImVec2(380, 180));
	ImGui::Begin(">> CREAR UN NODO <<", 0, window_flags);
	static char IP[MAX_IP];
	static char Puerto[MAX_PUERTO];
	static bool nodofull = false;
	static bool nodospv = false;
	ImGui::Checkbox("NODO FULL", &nodofull);
	ImGui::Checkbox("NODO SPV", &nodospv);
	ImGui::InputText("IP DEL SERVIDOR:", IP, sizeof(char) * MAX_IP);
	ImGui::InputText("PUERTO DEL SERVIDOR:", Puerto, sizeof(char) * MAX_PUERTO);

	if (ImGui::Button(" >> CREAR NODO << ") && ((nodofull == true) || (nodospv == true)) && ((sizeof(IP) != 0) && (sizeof(Puerto) != 0)))
	{
		if (nodofull == true)
		{
			RegistroNodo_t tempRegistro;
			tempRegistro.IP = IP;
			tempRegistro.TYPE = FULL;
			tempRegistro.PUERTO = atoi(Puerto);
			registros.push(tempRegistro);
		}

		if (nodospv == true)
		{
			RegistroNodo_t tempRegistro;
			tempRegistro.IP = IP;
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
	ImGui::SetNextWindowSize(ImVec2(380, 180));
	ImGui::Begin(">> CREAR CONEXION ENTRE NODOS <<", 0, window_flags);
	static char NODO1[MAX_ID];
	static char NODO2[MAX_ID];
	static bool fulltype[2] = { false };
	static bool spvtype[2] = { false };

	ImGui::InputText("NODO 1:", NODO1, sizeof(char) * MAX_ID);
	ImGui::Checkbox("NODO FULL 1 ", &fulltype[0]);
	ImGui::SameLine();
	ImGui::Checkbox("NODO SPV 1", &spvtype[0]);
	ImGui::Text("  ");
	ImGui::InputText("NODO 2:", NODO2, sizeof(char) * MAX_ID);
	ImGui::Checkbox("NODO FULL 2", &fulltype[1]);
	ImGui::SameLine();
	ImGui::Checkbox("NODO SPV 2", &spvtype[1]);


	if ((ImGui::Button(" >> CREAR << ")) && (verify(fulltype, spvtype, (string)NODO1, (string)NODO2)))
	{
		if (fulltype[0]) {
			RegistroNodo_t tempNodo1;
			tempNodo1.TYPE = FULL;
			tempNodo1.ID = atoi(NODO1);
			registros.push(tempNodo1);

		}
		else {
			RegistroNodo_t tempNodo1;
			tempNodo1.TYPE = SPV;
			tempNodo1.ID = atoi(NODO1);
			registros.push(tempNodo1);
		}

		if (fulltype[1]) {
			RegistroNodo_t tempNodo2;
			tempNodo2.TYPE = FULL;
			tempNodo2.ID = atoi(NODO2);
			registros.push(tempNodo2);
		}
		else {
			RegistroNodo_t tempNodo2;
			tempNodo2.TYPE = SPV;
			tempNodo2.ID = atoi(NODO2);
			registros.push(tempNodo2);
		}

		GUIQueue.push(GUIEvent::CrearConexion);
	}
	ImGui::End();

	/*************************************
	   VENTANA: ENVIAR MENSAJE
	**************************************/

	ImGui::SetNextWindowPos(ImVec2(20, 390));
	ImGui::SetNextWindowSize(ImVec2(380, 150));
	ImGui::Begin(">> ENVIAR MENSAJER A NODO <<", 0, window_flags);
	ImGui::Text("Inserte ID del nodo Emisor y su tipo (FULL o SPV)");

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

	/*************************************************************************************************
			FIN IMPRESION VENTANAS
	**************************************************************************************************/
	//Rendering
	ImGui::Render();

	al_clear_to_color(al_map_rgb(179, 255, 255));

	ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
	al_flip_display();
}

void GraphicF2::print_Bulletin(void)		//IMPORTANTE se llama depsues de haber creado un NewFrame en otra parte del programa
{

	ImGui::SetNextWindowPos(ImVec2(450, 10));
	ImGui::SetNextWindowSize(ImVec2(550, 600));
	ImGui::Begin(">>    BULLETIN BOARD   <<", 0, window_flags);

	//TOY PENSANDO EN VER Q CONVIENE SI COMUNICARNOS MEDIANTE UN ARCHIVO QUE NETWORKING LE 
	//NOS GUARDA EN LA COMPUTADORA Y ACA LO LEVANTAMOS Y VAMOS IMPRIMIENDO TODO
	//QUEDARIA COMO UN GRAN TEXTO Y PODES SCROLLEAR Y VER TODO 
	//QUEDARIA PIOLA GUARDAR LOS ESTADOS CON HORARIO 

	ImGui::End();
}

void GraphicF2::print_look4Veci(void)
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
	ImGui::SetNextWindowSize(ImVec2(400, 700));
	ImGui::Begin(">> COMUNICACION ENTRE NODOS <<", 0, window_flags);

	ParticipantesMsj_t ComEnProgreso = Comunicaciones.front();


	ImGui::Text(" ");

	ImGui::Text(" >> Seleccione un vecino << ");

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


	switch (ComEnProgreso.NodoEmisor.TYPE)
	{
	case FULL:
		char buf[32];
		ImGui::Text(">> RECIBIR <<");
		if (ImGui::TreeNode("Operaciones para recibir:"))
		{
			sprintf(buf, "Get blocks_");
			{
				if (ImGui::Selectable(buf, selected == GETBLOCKS_Grec))
					selected = GETBLOCKS_Grec;
			}
			sprintf(buf, "Get block headers_");
			{
				if (ImGui::Selectable(buf, selected == GETBLOCKHEADERS_Grec))
					selected = GETBLOCKHEADERS_Grec;
			}
			sprintf(buf, "Block_");
			{
				if (ImGui::Selectable(buf, selected == BLOCK_Grec))
					selected = BLOCK_Grec;
			}
			sprintf(buf, "Filter_");
			{
				if (ImGui::Selectable(buf, selected == FILTER_Grec))
					selected = FILTER_Grec;
			}
			ImGui::TreePop();
		}

		ImGui::Text(" ");
		ImGui::Text(">> ENVIAR <<");

		if (ImGui::TreeNode("Operaciones para enviar:"))
		{
			if (ImGui::TreeNode("Datos para transaccion"))
			{
				static char CantCoins[1000000000];
				ImGui::InputText("CANTIDAD:", CantCoins, IM_ARRAYSIZE(CantCoins));

				static char PKey[40000000];
				ImGui::InputText("PUBLIC KEY:", PKey, IM_ARRAYSIZE(PKey));

				sprintf(buf, "Transaccion_");
				{
					if (ImGui::Selectable(buf, selected == GETBLOCKS_Genv))
						selected = GETBLOCKS_Genv;
				}
				ImGui::TreePop();
			}

			sprintf(buf, "Get blocks_");
			{
				if (ImGui::Selectable(buf, selected == GETBLOCKS_Genv))
					selected = GETBLOCKS_Genv;
			}
			sprintf(buf, "Block_");
			{
				if (ImGui::Selectable(buf, selected == BLOCK_Genv))
					selected = BLOCK_Genv;
			}
			ImGui::TreePop();
		}

		break;

	case SPV:

		char bufSPV[32];
		ImGui::Text(">> RECIBIR <<");
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
		ImGui::Text(">> ENVIAR <<");

		if (ImGui::TreeNode("Operaciones para enviar:"))
		{
			if (ImGui::TreeNode("Datos para transaccion"))
			{
				static char CantCoins[1000000000];
				ImGui::InputText("CANTIDAD:", CantCoins, IM_ARRAYSIZE(CantCoins));

				static char PKey[40000000];
				ImGui::InputText("PUBLIC KEY:", PKey, IM_ARRAYSIZE(PKey));

				sprintf(bufSPV, "Transaccion");
				{
					if (ImGui::Selectable(buf, selected == GETBLOCKS_Genv))
						selected = GETBLOCKS_Genv;
				}
				ImGui::TreePop();
			}

			sprintf(bufSPV, "Get block headers");
			{
				if (ImGui::Selectable(buf, selected == GETBLOCKHEADERS_Genv))
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


	if (ImGui::Button(" >> ENVIAR MENSAJE << "))
	{

		Comunicaciones.front().selectedVecino = selectedN;
		GUIQueue.push(GUIEvent::EnviarMsj);

	}

	if (ImGui::Button(" >>  VOLVER A DASHBOARD  << "))
	{
		GUIQueue.push(GUIEvent::Back2Dashboard);
		cout << "VOLVER  A DASHBOARD BUTTON " << endl;
	}

	ImGui::End();



	//Rendering
	ImGui::Render();

	al_clear_to_color(al_map_rgb(179, 255, 255));

	ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
	al_flip_display();

}

ParticipantesMsj_t GraphicF2::getComunicacion(void)
{
	ParticipantesMsj_t Comunicacion = Comunicaciones.front();		//Sabemos que hay una comunicacion en la cola porque lo verifico verify()
	Comunicaciones.pop();

	return Comunicacion;
}


bool GraphicF2::verify(uint ExisteEsteNodo, bool esUnNodoSPV)
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


	/*{
		RegistroNodo_t NodoEmisor;
		std::map<unsigned int, Neighbour2> NodosVecinos;
		std::string mensaje;
		std::string vecinos;	//Esto se usa para la funcion combo de ImGui
		int selectedVecino;
	*/

	Neighbour neighbourTemp = { "0001", 80 };

	tempParticipantes.NodosVecinos.insert(pair<unsigned int, Neighbour>(1, neighbourTemp));

	Neighbour neighbourTemp2 = { "0002", 80 };

	tempParticipantes.NodosVecinos.insert(pair<unsigned int, Neighbour>(2, neighbourTemp2));

	Neighbour neighbourTemp3 = { "0003", 80 };

	tempParticipantes.NodosVecinos.insert(pair<unsigned int, Neighbour>(3, neighbourTemp3));



	tempParticipantes.NodoEmisor.ID = 2222;
	tempParticipantes.NodoEmisor.IP = "3333";
	tempParticipantes.NodoEmisor.PUERTO = 80;

	string tempIDVecino;
	std::map<unsigned int, Neighbour>::iterator it = tempParticipantes.NodosVecinos.begin();


	for (it = tempParticipantes.NodosVecinos.begin(); it != tempParticipantes.NodosVecinos.end(); ++it)
	{
		tempIDVecino = "IP: " + it->second.IP + " - PORT: " + to_string(it->second.port);
		cout << tempIDVecino << endl;
		tempParticipantes.vecinos.push_back(tempIDVecino);
	}


	tempIDVecino.append("\0");

	this->Comunicaciones.push(tempParticipantes);

	//Comunicaciones = tempParticipantes;

	/*
	for (auto itnodo : FullNodes)
	{
		if (itnodo.getID() == ExisteEsteNodo)		//Si encontramos ese ID entonces existe nodo proseguimos a buscar sus vecinos
		{
			ParticipantesMsj_t tempParticipantes;

			tempParticipantes.NodosVecinos = itnodo.getNeighbours();
			tempParticipantes.NodoEmisor.ID = itnodo.getID();
			tempParticipantes.NodoEmisor.IP = itnodo.getIP();
			tempParticipantes.NodoEmisor.PUERTO = itnodo.getPort();
			string tempIDVecino;
			for (auto vecinos : tempParticipantes.NodosVecinos)
			{
				tempIDVecino.append("IP: " + vecinos.second.IP + " - PORT: "+ to_string(vecinos.second.port) + "\0");
			}
			tempIDVecino.append("\0");

			this->Comunicaciones.push(tempParticipantes);
			ret = true;
		}
	}

	for (auto itnodo : SPVNodes)
	{
		if (itnodo.getID() == ExisteEsteNodo)		//Si encontramos ese ID entonces existe nodo proseguimos a buscar sus vecinos
		{
			ParticipantesMsj_t tempParticipantes;

			tempParticipantes.NodosVecinos = itnodo.getNeighbours();
			tempParticipantes.NodoEmisor.ID = itnodo.getID();
			tempParticipantes.NodoEmisor.IP = itnodo.getIP();
			tempParticipantes.NodoEmisor.PUERTO = itnodo.getPort();
			string tempIDVecino;
			for (auto vecinos : tempParticipantes.NodosVecinos)
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
	*/

	return true;
}

bool GraphicF2::verify(bool* full, bool* spv, string nodo1, string nodo2)
{
	if ((spv[0] != spv[1]) && (!nodo1.empty() && !nodo2.empty()))
	{
		return true;
	}
	else
		return false;
}


void GraphicF2::print_Error(void)
{
	ImGui_ImplAllegro5_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(400, 270));
	ImGui::SetNextWindowSize(ImVec2(300, 70));

	ImGui::Begin("No se puede hacer esa operacion", 0, window_flags);

	if (ImGui::Button("Quit"))
	{
		GUIQueue.push(GUIEvent::Quit);
	}

	ImGui::End();

	ImGui::Render();

	al_clear_to_color(al_map_rgb(179, 255, 255));

	ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
	al_flip_display();
}


RegistroNodo_t GraphicF2::getRegistro(void)
{
	RegistroNodo_t returning = registros.front();

	registros.pop();
	return returning;
}

bool GraphicF2::AllegroInit()
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

bool GraphicF2::ImguiInit(void)
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
