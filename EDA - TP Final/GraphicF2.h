
/***************************************************************************************
*  LA CLASE GRAFICA ESTA CONTENIDA EN GUIEVENT GENERATOR
*  YA QUE ES LA PARTE GRAFICA QUIEN GENERA LOS EVENTOS DE LA GUI
*  ASI GUIEventGenerator.cpp LLAMA A LAS FUNCIONES DE GRAPHICF2 PARA GENERAR EVENTOS
****************************************************************************************/


#ifndef GRAPHICF2_H
#define GRAPHICF2_H

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
#include "FullNode.h"
#include "SPVNode.h"
#include "Structs.h"

/* DEFINES */
#define SIZE_SCREEN_X 2000
#define SIZE_SCREEN_Y 700
#define MAX_IP 15
#define MAX_ID 15
#define MAX_PUERTO 10
#define MAX_MSJ 500
#define SPV 1
#define FULL 2
#define MAX_VECINOS 50

#define COLOR al_map_rgb(100,200,240)

enum STATES { DASHBOARD_G, LOOK4VECI_G, SHWNODOS_G, SHWERROR_G };



/* Filesystems namespace */
namespace fs = boost::filesystem;
using json = nlohmann::json;

class GraphicF2
{
public:
	GraphicF2();
	~GraphicF2();
	bool GetError();
	bool hayEvento(unsigned int estadoActualdeFSM); //Devuelve si durante una de las impresiones de displays hubo un evento (botones y cerrar pesataña)
	std::vector<RegistroNodo_t>* getNodoArray(void);
	GUIEvent getEvent();

	/*  GETTERS  */
	RegistroNodo_t getRegistro(void);
	ParticipantesMsj_t getComunicacion(void);
	std::ofstream BulletinFile;

private:
	/* FUNCIONES DE GraphicF2 */
	bool AllegroInit();
	bool ImguiInit();

	/* FUNCIONES DE IMPRESION */
	void print_current_state(unsigned int CurrentState);
	void print_Dashboard();//Imprime el menu principal al comienzo del programa
	void print_Error();
	void print_look4Veci();
	void print_Bulletin();
	void print_Nodos();
	void print_Acciones();

	bool verify(bool*, bool*, string, string);		//Veryfy para creacion de nodo. Si es SPV no puede conectarse a otro SPV. IP PUERTO no pueden ser campos vacios. 
	bool verify(uint, bool esSPV);			//Verify para BUSCAR VECINOS rellena el objeto ParticipantesMsj con info de emisor y sus vecinos y lo guarda en la queue
	bool verify(unsigned int, string, string, int);		//Verify para ENVIAR MENSAJE relllena los campos faltantes del objeto ParticipantesMsj con el mensaje (verificado) y el vecino receptor

	/* COLA DE EVENTOS QUE LEVANTA EL GUI EVENT GENERATOR */
	std::queue<GUIEvent> GUIQueue;


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

	/*******************
	 *      NODOS      *
	********************/
	std::queue<RegistroNodo_t> registros;		//Registros temporales por eso cola

	std::queue<ParticipantesMsj_t> Comunicaciones;

	vector<RegistroNodo_t> NodosArray; 


};

#endif //GRAPHICF2