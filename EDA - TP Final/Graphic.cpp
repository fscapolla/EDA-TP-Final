#include "Graphic.h"


Graphic::Graphic(Blockchain& pBchain_): pBchain(pBchain_)
{
	if (AllegroInit() && ImguiInit())
	{
		al_register_event_source(this->queue, al_get_display_event_source(display));
		al_register_event_source(this->queue, al_get_mouse_event_source());
		al_register_event_source(this->queue, al_get_keyboard_event_source());

		window_flags |= ImGuiWindowFlags_NoResize;
		InputState = false;
		Error = false;
		close = false;
		readString = "";
		EstadoActual = Estado::MainMenu;
		
	}
	else
	{
		Error = true;
	}
}

Graphic::~Graphic()
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

bool Graphic::GetError()
{
	return Error;
}

bool Graphic::RunningOne()
{
	if (hayEvento())
	{
		Dispatch();
	}
	return (!close);
}

bool Graphic::hayEvento(void)
{
	while (al_get_next_event(queue, &ev))
	{
		ImGui_ImplAllegro5_ProcessEvent(&ev);
	}

	if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
	{
		EventQueue.push(Evento::Close);
	}

	print_current_state(EstadoActual);  //Devuelve true si hubo un evento (Usuario presiono un boton)
												//Todas las funciones de impresion BUSCAN eventos y las guardan en EventQueue				

	return !EventQueue.empty();		//Si hay evento devuelve true
}

void Graphic::Dispatch(void)			//Dispatch lee los eventos y cambia estados
{
	if (!EventQueue.empty())
	{
		switch (EventQueue.front())
		{
		case Evento::Close:
			close = true;			//No hay switch case se cierra el display de una
			EventQueue.pop();
			break;

		case Evento::gotoMainMenu:
			switch (EstadoActual)
			{
			case Estado::SelectingBlocks:
			case Estado::ShowingError:
			case Estado::InfoReady:
				EstadoActual = Estado::MainMenu;
				flushVariables();
				break;
			default:
				break;
			}
			break;

		case Evento::GetInfo:
			if (EstadoActual == Estado::SelectingBlocks)		//No hago switch pq solo en un estado puede pasar el evento get info
			{
				EstadoActual = Estado::Loading;
				InputState = true;
				EventQueue.pop();
			}
			break;

		case Evento::DirectorioInput:
			if (EstadoActual == Estado::MainMenu)
			{
				EstadoActual = Estado::SelectingBlocks;
				EventQueue.pop();
			}
			break;

		case Evento::Error:
			switch (EstadoActual)
			{
			case Estado::MainMenu:
			case Estado::Loading:
				EstadoActual = Estado::ShowingError;
				EventQueue.pop();
				break;
			default:
				break;
			}
			break;

		case Evento::Success:
			if (EstadoActual == Estado::Loading)
			{
				EstadoActual = Estado::InfoReady;
				EventQueue.pop();
			}
			break;

		case Evento::ShowResult:
			if (EstadoActual == Estado::InfoReady)
			{
				EstadoActual = Estado::RequestedInfo;
				EventQueue.pop();
			}
			break;

		default:
			break;
		}
	}
}

void Graphic::print_current_state(Estado CurrentState)
{
	switch (CurrentState)
	{
	case Estado::MainMenu:					
		print_MainMenu();
		break;

	case Estado::SelectingBlocks:
		print_SelectBlocks();
		break;

	case Estado::ShowingError:
		print_Error();
		break;

	case Estado::Loading:
		print_Loading();
		break;

	case Estado::InfoReady:
		print_Done();
		break;

	case Estado::RequestedInfo:
		print_info();
		break;

	default:
		break;
	}
}

void Graphic::print_MainMenu()
{
	ImGui_ImplAllegro5_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(200, 10));
	ImGui::SetNextWindowSize(ImVec2(600, 150));

	ImGui::Begin("Inserte el camino al directorio con los bloques", 0, window_flags);
	static char paths[MAX_PATH];

	ImGui::InputText("Directorio", paths, sizeof(char) * MAX_PATH);
	if (ImGui::Button("Iniciar"))
	{
		EventQueue.push(Evento::DirectorioInput);		//Tenemos el directorio listo 
		path.assign(paths);
		directoryName.assign(paths);
		look4BlocksPath();
	}

	ImGui::End();

	//Rendering
	ImGui::Render();

	al_clear_to_color(al_map_rgb(211, 211, 211));

	ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
	al_flip_display();
}

void Graphic::look4BlocksPath()
{
	fs::path bPath(directoryName);
	if (exists(bPath) && is_directory(bPath))
	{
		for (fs::directory_iterator iterator(bPath); iterator != fs::directory_iterator(); iterator++)
		{
			if (iterator->path().filename().string() == "blockChain.json")
			{
				std::cout << iterator->path().string() << std::endl;
				if (!pBchain.saveBlockInfo(iterator->path().filename().string())) {
				}
				else {
					EventQueue.push(Evento::Error);
				}				
			}
		}
	}
	else
	{
		EventQueue.push(Evento::Error);
	}
}

void Graphic::print_SelectBlocks()
{
	ImGui_ImplAllegro5_NewFrame();
	ImGui::NewFrame();

	int i;

	ImGui::SetNextWindowPos(ImVec2(200, 10));
	ImGui::SetNextWindowSize(ImVec2(600, 650));

	ImGui::Begin("Seleccione 1 bloque", 0, window_flags);

	//Checkbox con imgui
	static bool checks[MAX_BLOCKS] = { false };
	for (i = 0; i < (pBchain.getBlocksArr()).size(); i++)
	{
		ImGui::Checkbox((pBchain.getBlocksArr())[i].getBlockID().c_str(), &checks[i]);
	}

	if (ImGui::Button("Buscar Info"))
	{
		for (i = 0; i < (pBchain.getBlocksArr()).size(); i++) {
			if (checks[i] == true)
			{
				selectedBlock.push_back((pBchain.getBlocksArr())[i]);
				i = pBchain.getBlocksArr().size();		//Solo selecciono uno
			}
		}	
		EventQueue.push(Evento::GetInfo);
	}

	if (ImGui::Button("Volver al menu prinicpal"))
	{
		EventQueue.push(Evento::gotoMainMenu);
	}

	ImGui::End();

	//Rendering
	ImGui::Render();

	al_clear_to_color(al_map_rgb(211, 211, 211));

	ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());

	al_flip_display();
}

void Graphic::print_Error(void)
{
	ImGui_ImplAllegro5_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(400, 270));
	ImGui::SetNextWindowSize(ImVec2(300, 70));

	ImGui::Begin("No existe el camino con blockes", 0, window_flags);

	if (ImGui::Button("Quit"))
	{
		EventQueue.push(Evento::Close);
		//EventoActual = Evento::Close;
	}

	ImGui::SameLine();
	if (ImGui::Button("Volver al menu prinicpal"))
	{
		EventQueue.push(Evento::gotoMainMenu);
		//EventoActual = Evento::gotoMainMenu;
	}

	ImGui::End();

	ImGui::Render();

	al_clear_to_color(al_map_rgb(211, 211, 211));

	ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
	al_flip_display();
}

void Graphic::flushVariables() {
	InputState = false;
	Error = false;
	close = false;
	path = "";
	directoryName = "";
	pBchain.getBlocksArr().clear();
	while (!EventQueue.empty())
		EventQueue.pop();	
}

void Graphic::print_Loading(void)
{
	
	ImGui_ImplAllegro5_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(200, 10));
	ImGui::SetNextWindowSize(ImVec2(600, 100));

	ImGui::Begin("LOADING", 0, window_flags);
	ImGui::Text("Buscando informacion...");

	ImGui::End();

	ImGui::Render();

	al_clear_to_color(al_map_rgb(211, 211, 211));

	ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
	al_flip_display();
	
}

void Graphic::print_Done(void)
{
	ImGui_ImplAllegro5_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(250, 100));
	ImGui::SetNextWindowSize(ImVec2(350, 300));
	ImGui::Begin("Elija acciones sobre Block:");

	if (sizeof(selectedBlock) != 0)
	{
		static bool Actions[4];		//4 acciones posibles

		ImGui::Checkbox("Show block information: " , &Actions[SHOWINFO]);

		ImGui::Checkbox("Calculate Merkle Root:", &Actions[CALCULATEMERKLE]);

		ImGui::Checkbox("Validate Merkle Root:", &Actions[VALIDATEMERKLE]);

		ImGui::Checkbox("Show Merkle Tree:", &Actions[SHOWMERKLE]);

		if (ImGui::Button(" START: "))
		{
			EventQueue.push(Evento::ShowResult);
			
			if (Actions[CALCULATEMERKLE])
			{
				//Calculamos merkle root y guardamos en variable para impresion
				//selectedBlock[0].createMerkleRoot();

			}
			int i;
			for (i = 0; i < 4; i++)
				ActionsArray[i] = Actions[i];
		}
	}
	else
	{
		ImGui::Text("Ningun Bloque se ha seleccionado");
	}

	if (ImGui::Button(" Quit "))
	{
		EventQueue.push(Evento::Close);
	}
	ImGui::SameLine();
	if (ImGui::Button("Volver al menu prinicpal"))
	{
		EventQueue.push(Evento::gotoMainMenu);
	}

	ImGui::End();

	ImGui::Render();

	al_clear_to_color(al_map_rgb(211, 211, 211));

	ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());

	al_flip_display();
}

void Graphic::print_info(void) {
	ImGui_ImplAllegro5_NewFrame();
	ImGui::NewFrame();

	if (ActionsArray[SHOWINFO])
	{
		ImGui::SetNextWindowPos(ImVec2(100, 10));
		ImGui::SetNextWindowSize(ImVec2(350, 300));
		ImGui::Begin(" INFORMACION DEL BLOQUE ");

		ImGui::Text("Block ID: %s", selectedBlock[0].getBlockID());

		ImGui::Text("Previous Block ID: %s", selectedBlock[0].getPrevBlovkID());

		ImGui::Text("Cantidad de transacciones: %u", selectedBlock[0].getNtx());

		ImGui::Text("Numero de bloque: %u",selectedBlock[0].getHeight());

		//ImGui::Text("Nounce: %s", selectedBlock[0].getNonce());

		ImGui::End(); 

	}

	if (ActionsArray[CALCULATEMERKLE])
	{
		ImGui::SetNextWindowPos(ImVec2(500, 10));
		ImGui::SetNextWindowSize(ImVec2(200, 70));
		ImGui::Begin(" CALCULO DE MERKLE ROOT");

		ImGui::Text("Merkel Root: %s",selectedBlock[0].getCalculatedMerkleRoot());		//Se calculó merkleRoot al presionar boton start
		ImGui::End();
	}

	if (ActionsArray[VALIDATEMERKLE])
	{
		ImGui::SetNextWindowPos(ImVec2(700, 10));
		ImGui::SetNextWindowSize(ImVec2(200, 70));
		ImGui::Begin(" VALIDACION DE MERKLE ROOT");

		if (selectedBlock[0].createMerkleTree()) 
		{
			ImGui::Text(" MERKLE ROOT IS VALID ");
		}
		else
		{
			ImGui::Text(" MERKLE ROOT IS NOT VALID ");
		}
		ImGui::End();
	}


	if (ActionsArray[SHOWMERKLE])
	{

	}

	ImGui::Render();

	al_clear_to_color(al_map_rgb(211, 211, 211));
 
	ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());

	al_flip_display();


}

/* FUNCIONES PARA USER */
void Graphic::processingRequest() // Le comunica a la gui que se llamo al algoritmo
{
	WorkInProgress = true;
}

bool Graphic::inputReady() // Te dice si el user ya seleccionó las imagenes deseadas
{
	return InputState;
}

void Graphic::error() // Le comunica a la gui que se realizó la operación exitosamente
{
	Result = false;
	WorkInProgress = false;
	EventQueue.push(Evento::Error);
}

bool Graphic::AllegroInit()
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

bool Graphic::ImguiInit(void)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	display = al_create_display(SIZE_SCREEN_X, SIZE_SCREEN_Y);
	if (display)
	{
		al_set_window_position(display, 500, 100); //posicion del menu

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		ImGui_ImplAllegro5_Init(display);
	}
	else
		return false;

	return true;
}

void Graphic::success() // Le comunica a la gui que se realizó la operación exitosamente
{
	Result = true;
	WorkInProgress = false;
	InputState = false;
	EventQueue.push(Evento::Success);
}
