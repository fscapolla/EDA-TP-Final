

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

bool GraphicF2::hayEvento(implStates EstadoActual)
{
	while (al_get_next_event(queue, &ev))
	{
		ImGui_ImplAllegro5_ProcessEvent(&ev);
	}

	if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
	{
		EventQueue.push(GUIEvent::Quit);
	}

	print_current_state(EstadoActual);  //Devuelve true si hubo un evento (Usuario presiono un boton)
												//Todas las funciones de impresion BUSCAN eventos y las guardan en EventQueue			

	return !EventQueue.empty();		//Si hay evento devuelve true
}

GUIEvent GraphicF2::getEvent()
{
	GUIEvent EventoParaEvGenerator = EventQueue.front();
	EventQueue.pop();

	return EventoParaEvGenerator;
}

void GraphicF2::print_current_state(implStates CurrentState)
{
	switch (CurrentState)
	{
	case  MostrandoMenu:
		print_MainMenu();
		break;

	case ShowingError:
		print_Error();
		break;

	case Loading:
//		print_Loading();
		break;

	default:
		break;
	}
}

void GraphicF2::print_MainMenu()
{
	ImGui_ImplAllegro5_NewFrame();
	ImGui::NewFrame();

	/******************************
		VENTANA: CREAR UN NODO
	******************************/
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
			EventQueue.push(GUIEvent::CrearNodoFULL);
		}

		if (nodospv == true)
		{
			EventQueue.push(GUIEvent::CrearNodoSPV);
		}

		//ACA ENVIAR DATOS A CREACION DE NODOS?
	}

	ImGui::End();

	/*************************************
	VENTANA: CREAR CONEXION ENTRE NODOS
	**************************************/

	ImGui::SetNextWindowPos(ImVec2(20, 200));
	ImGui::SetNextWindowSize(ImVec2(380, 180));
	ImGui::Begin(">> CREAR CONEXION ENTRE NODOS <<", 0, window_flags);
	static char NODO1[MAX_IP];
	static char NODO2[MAX_IP];
	static bool fulltype[2] = {false };
	static bool spvtype[2] = { false };

	ImGui::InputText("NODO 1:", NODO1, sizeof(char) * MAX_IP);
	ImGui::Checkbox("NODO FULL", &fulltype[0]);
	ImGui::SameLine();
	ImGui::Checkbox("NODO SPV", &spvtype[0]);
	ImGui::Text("  ");
	ImGui::InputText("NODO 2:", NODO1, sizeof(char) * MAX_IP);
	ImGui::Checkbox("NODO FULL", &fulltype[1]);
	ImGui::SameLine();
	ImGui::Checkbox("NODO SPV", &fulltype[1]);


	if ((ImGui::Button(" >> CREAR NODO << "))&& (verify(fulltype,spvtype,(string)NODO1,(string)NODO2)))
	{
		EventQueue.push(GUIEvent::CrearConexion);
	}
	ImGui::End();

	/*************************************
	   VENTANA: ENVIAR MENSAJE	
	**************************************/

	ImGui::SetNextWindowPos(ImVec2(20, 390));
	ImGui::SetNextWindowSize(ImVec2(380, 150));
	ImGui::Begin(">> ENVIAR MENSAJER A NODO <<", 0, window_flags);
	static char remitente[MAX_IP];
	static bool type[2] = { false };

	ImGui::InputText("NODO REMITENTE:", remitente, sizeof(char) * MAX_IP);

	if (ImGui::Button(" >> BUSCAR VECINOS << "))
	{
		//EventQueue.push(GUIEvent::BuscarVecinos);
		// me trabe
	}
	ImGui::End();


	//Rendering
	ImGui::Render();

	al_clear_to_color(al_map_rgb(179, 255, 255));

	ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
	al_flip_display();
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
		EventQueue.push(GUIEvent::Quit);
	}

	ImGui::End();

	ImGui::Render();

	al_clear_to_color(al_map_rgb(179, 255, 255));

	ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
	al_flip_display();
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
