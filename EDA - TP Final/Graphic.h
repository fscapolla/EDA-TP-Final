#pragma once

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
#include <allegro5/allegro_font.h>
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

/* DEFINES */
#define MAX_PATH 500
#define MAX_BLOCKS 50
#define SIZE_SCREEN_X 2350
#define SIZE_SCREEN_Y 700
#define LEVEL_INCREASE_Y 50
#define LEVEL_INCREASE_X 75
#define INITIAL_X 0
#define INITIAL_Y 500

/* EVENT QUEUE */
enum class Evento { Close , gotoMainMenu, DirectorioInput, Error, GetInfo, ShowResult, Success };
enum class Estado { MainMenu, InfoReady, Loading, SelectingBlocks, ShowingError, RequestedInfo, ShowingBlockInfo, JFiles};
enum {SHOWINFO, CALCULATEMERKLE, VALIDATEMERKLE, SHOWMERKLE};
/* Filesystems namespace */
namespace fs = boost::filesystem;
using json = nlohmann::json;

class Graphic
{
public:
	Graphic(Blockchain& pBchain);
	~Graphic();
	bool GetError();
	bool RunningOne();
	bool inputReady();
	void success(); // Le comunica a la gui que se realizó la operación exitosamente
	void processingRequest();
	void error();
	

private:
	/* FUNCIONES DE GRAPHICS */
	bool AllegroInit();
	bool ImguiInit();
	bool hayEvento(); //Devuelve si durante una de las impresiones de displays hubo un evento (botones y cerrar pesataña)
	void Dispatch(); //Se llama despues de hayEvento y acciona
	bool look4BlocksPath(string ChosenFile);
	void loadFiles();
	void flushVariables();
	void printLevel(uint altura, uint Hojas, uint height, std::vector<string> nodos);

	/* FUNCIONES DE IMPRESION */
	void print_current_state(Estado);	//Dependiendo del estado del programa imprime la interfaz deseada
	void print_MainMenu();//Imprime el menu principal al comienzo del programa
	void print_SelectBlocks();
	void print_Loading();
	void print_Done(); //Imprime los resultados de la compression/decompression
	void print_Error();
	void print_info();
	void print_jsons();
	//bool Graphic::print_chooseFile();

	/* VARIABLES DE ALLEGRO */
	ALLEGRO_EVENT_QUEUE* queue;
	ALLEGRO_EVENT ev;
	ALLEGRO_DISPLAY* display;

	/* VARIABLES DE IMGUI */
	ImGuiWindowFlags window_flags;

	/* VARIABLES DE mini-FSM*/
	std::queue<Evento> EventQueue;
	Evento EventoActual;
	Estado EstadoActual;
	bool Error;
	bool close;
	bool InputState;
	bool stillRunning;
	bool selectImages;
	bool correctlyInitialized;
	bool WorkInProgress;
	bool Result;

	/* VARIABLES DE INPUT */
	std::string path;
	std::string directoryName;
	std::vector<std::string> Files;
	vector<Block> selectedBlock;

	std::string readString;
	bool ValidationMerkleRoot; 
	bool ActionsArray[4];


	/*Puntero a BlockChain*/

	Blockchain& pBchain;


};

