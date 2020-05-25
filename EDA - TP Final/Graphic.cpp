#include "Graphic.h"
unsigned char clickedBlock(bool* checks, size_t size);
unsigned char numSelectedBlocks(bool* checks, size_t size);


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
		ValidationMerkleRoot = false;
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
			/*switch (EstadoActual)
			{
			case Estado::RequestedInfo:
			case Estado::SelectingBlocks:
			case Estado::ShowingError:
			case Estado::InfoReady:*/
				EstadoActual = Estado::MainMenu;
				flushVariables();
			/*	break;
			default:
				break;
			}
			break;*/

		case Evento::GetInfo:
			if (EstadoActual == Estado::SelectingBlocks)		//No hago switch pq solo en un estado puede pasar el evento get info
			{
				EstadoActual = Estado::Loading;
				InputState = true;
				EventQueue.pop();
			}
			break;

		case Evento::DirectorioInput:
			if (EstadoActual == Estado::JFiles)
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
	case Estado::JFiles:
		print_jsons();
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
				//Tenemos el directorio listo 
		path.assign(paths);
		directoryName.assign(paths);
		EstadoActual = Estado::JFiles;
		loadFiles();
	}

	ImGui::End();

	//Rendering
	ImGui::Render();

	al_clear_to_color(al_map_rgb(179, 255, 255));

	ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
	al_flip_display();
}

bool Graphic::look4BlocksPath(string ChosenFile)
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
					EventQueue.push(Evento::Error);
					return false;
				}				
			}
		}
	}
	else
	{

		EventQueue.push(Evento::Error);
		return false;
	}
}

void Graphic::loadFiles() {

	fs::path bPath(directoryName);
	for (boost::filesystem::directory_iterator itr(bPath); itr != boost::filesystem::directory_iterator(); itr++) {

		if (!itr->path().extension().string().find(".json"))
		{
   
			Files.push_back(itr->path().filename().string());
		}
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
	unsigned char box = clickedBlock(checks, (pBchain.getBlocksArr()).size());
	for (i = 0; i < pBchain.getBlocksSize(); i++)
	{
		
		ImGui::Checkbox((pBchain.getBlocksArr())[i].getBlockID().c_str(), &checks[i]);
		if (numSelectedBlocks(checks, (pBchain.getBlocksArr()).size()) > 1)
			checks[box] = false;

		
	}

	if (ImGui::Button("Buscar Info"))
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
		if(AlgunoSelected)
			EventQueue.push(Evento::GetInfo);
	}
	
	if (ImGui::Button("Volver al menu prinicpal"))
	{
		EventQueue.push(Evento::gotoMainMenu);
	}

	ImGui::End();

	//Rendering
	ImGui::Render();

	al_clear_to_color(al_map_rgb(179, 255, 255));

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

	al_clear_to_color(al_map_rgb(179, 255, 255));

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
	selectedBlock.clear();
	Files.clear();
	while (!EventQueue.empty())
		EventQueue.pop();	
}

void Graphic::print_jsons(void) {

	ImGui_ImplAllegro5_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(200, 10));
	ImGui::SetNextWindowSize(ImVec2(600, 650));
	int i;

	ImGui::Begin("Seleccione archivo JSON", 0, window_flags);

	static bool checks[MAX_BLOCKS] = { false };
	unsigned char box = clickedBlock(checks, Files.size());
	for (i = 0; i < Files.size(); i++)
	{
		std::size_t pos = Files[i].find(".");
		ImGui::Checkbox(Files[i].substr(0,pos).c_str(), &checks[i]);
		if (numSelectedBlocks(checks, Files.size()) > 1)
			checks[box] = false;


	}
	if (ImGui::Button("Search Blocks"))
	{
		
		if (numSelectedBlocks(checks, Files.size())) {

			if (look4BlocksPath(Files[clickedBlock(checks, Files.size())]))
				EventQueue.push(Evento::DirectorioInput);
			else
				EventQueue.push(Evento::gotoMainMenu);
		}
	}

	if (ImGui::Button("Volver al menu prinicpal"))
	{
		EventQueue.push(Evento::gotoMainMenu);
	}

	ImGui::End();

	//Rendering
	ImGui::Render();

	al_clear_to_color(al_map_rgb(179, 255, 255));

	ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());

	al_flip_display();





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

	al_clear_to_color(al_map_rgb(179, 255, 255));

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

		ImGui::Checkbox("Show block information " , &Actions[SHOWINFO]);

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
			EventQueue.push(Evento::ShowResult);
			
			if (Actions[CALCULATEMERKLE])
			{
				//Calculamos merkle tree y guardamos si es valido o no
				ValidationMerkleRoot = selectedBlock[0].createMerkleTree();
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

	al_clear_to_color(al_map_rgb(179,255,255));

	ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());

	al_flip_display();
}

void Graphic::print_info(void) {
	ImGui_ImplAllegro5_NewFrame();
	ImGui::NewFrame();

	if (ActionsArray[SHOWINFO])
	{
		ImGui::SetNextWindowPos(ImVec2(50, 10));
		ImGui::SetNextWindowSize(ImVec2(350, 150));
		ImGui::Begin(" INFORMACION DEL BLOQUE ");

		ImGui::Text("Block ID: %s", selectedBlock[0].getBlockID().c_str());

		ImGui::Text("Previous Block ID: %s", selectedBlock[0].getPrevBlovkID().c_str());

		ImGui::Text("Cantidad de transacciones: %u", selectedBlock[0].getNtx());

		ImGui::Text("Numero de bloque: %u",selectedBlock[0].getHeight());

		ImGui::Text("Nounce: %u", selectedBlock[0].getNonce());

		ImGui::End(); 

	}

	if (ActionsArray[CALCULATEMERKLE])
	{
		ImGui::SetNextWindowPos(ImVec2(420, 10));
		ImGui::SetNextWindowSize(ImVec2(230, 70));
		ImGui::Begin(" CALCULO DE MERKLE ROOT");

		ImGui::Text("Merkel Root: %s",selectedBlock[0].getCalculatedMerkleRoot().c_str());		//Se calculó merkleRoot al presionar boton start
		ImGui::End();
	}

	if (ActionsArray[VALIDATEMERKLE])
	{
		ImGui::SetNextWindowPos(ImVec2(670, 10));
		ImGui::SetNextWindowSize(ImVec2(200, 70));
		ImGui::Begin(" VALIDACION DE MERKLE ROOT");

		if (ValidationMerkleRoot) 
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

	

	if (ImGui::Button(" Quit "))
	{
		EventQueue.push(Evento::Close);
	}
	ImGui::SameLine();
	if (ImGui::Button("Volver al menu prinicpal"))
	{
		EventQueue.push(Evento::gotoMainMenu);
	}


	ImGui::Render();

	al_clear_to_color(al_map_rgb(179, 255, 255));

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

unsigned char clickedBlock(bool* checks, size_t size) {

	int i;
	unsigned char count = 0;
	for (i = 0; i < size; i++) {
		if (*(checks+i))
			return count;
		count++;
	}
	return count;
}
unsigned char numSelectedBlocks(bool* checks, size_t size) {

	int i;
	unsigned char count = 0;
	for (i = 0; i < size; i++) {
		if (*(checks + i))
			count++;
	}
	return count;
}