#pragma once
/***************************************************************************************
*  LA CLASE GRAFICA ESTA CONTENIDA EN GUIEVENT GENERATOR
*  YA QUE ES LA PARTE GRAFICA QUIEN GENERA LOS EVENTOS DE LA GUI 
*  ASI GUIEventGenerator.cpp LLAMA A LAS FUNCIONES DE GRAPHICF2 PARA GENERAR EVENTOS
****************************************************************************************/

#include<vector>
#include<string>
#include <list> 
#include <iterator>
#include <stdio.h>
#include <iostream>
#include <iostream>
#include <fstream>
#include <queue>

/* ALLEGRO */
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h> 
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_acodec.h> 

/* IMGUI */
#include "imgui_files/imgui.h"
#include "imgui_files/imgui_impl_allegro5.h"

/* BOOST */
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

/* BLOCKS */
#include "Block.h"
#include "Blockchain.h"
#include "json.hpp"

/* FSM */ 
#include "FSM.h"			//La incluyo para tener los implStates

/* DEFINES */
#define SIZE_SCREEN_X 2000
#define SIZE_SCREEN_Y 700
#define MAX_IP 15
#define MAX_PUERTO 10


/* EVENTOS FASE 2 */
enum class GUIEvent { CrearNodoSPV, CrearNodoFULL, CrearConexion, MostrarNodos, BuscarVecinos, EnviarMsj, MostrarMsj, AccionDone, NoEvent, Quit };


/* Filesystems namespace */
namespace fs = boost::filesystem;
using json = nlohmann::json;

class GraphicF2
{
public:
	GraphicF2();
	~GraphicF2();
	bool GetError();
	bool hayEvento(implStates estadoActualdeFSM); //Devuelve si durante una de las impresiones de displays hubo un evento (botones y cerrar pesataña)
	GUIEvent getEvent(); 
	  
private:
	/* FUNCIONES DE GraphicF2 */
	bool AllegroInit();
	bool ImguiInit();

	/* FUNCIONES DE IMPRESION */
	void print_current_state(implStates CurrentState);
	void print_MainMenu();//Imprime el menu principal al comienzo del programa
	void print_Error();
	bool verify(bool*, bool*, string, string);
	/* COLA DE EVENTOS QUE LEVANTA EL GUI EVENT GENERATOR */
	std::queue<GUIEvent> EventQueue;


	/* VARIABLES DE ALLEGRO */
	ALLEGRO_EVENT_QUEUE* queue;
	ALLEGRO_EVENT ev;
	ALLEGRO_DISPLAY* display;

	/* VARIABLES DE IMGUI */
	ImGuiWindowFlags window_flags;

	/* VARIABLES DE INPUT */
	std::string path;
	std::string directoryName;
	std::vector<std::string> Files;
	vector<Block> selectedBlock;

	std::string readString;

	bool Error;


};

