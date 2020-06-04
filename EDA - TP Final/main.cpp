#include <iostream>
#include "json.hpp"
#include "Block.h"
#include "Blockchain.h"
#include "FSM.h"
#include "EventHandling.h"
#include "GUIEventGenerator.h"


#include <ios>

int main(void)
{
    uint myCounter = 0;
 
	FSM fsm;
	GUIEventGenerator evTipoGUI(fsm.getSPVArrayPTR(), fsm.getFULLArrayPTR());	//generador de UN tipo de eventos 

	if (!evTipoGUI.getGraphicInstallationError())
	{
		mainEventGenerator eventGen;	//generador de eventos de TODO el programa

		//guiEventGenerator guiEvGen;
		//netwEventGenerator netwEvGen;

		eventGen.attach(&evTipoGUI);	//registro fuente de eventos

		bool quit = false;
		do
		{
			genericEvent* ev;
			ev = eventGen.getNextEvent(fsm.state4Graphic);  //ACA
			if (ev != nullptr)
			{
				if (ev->getType() == Quit)
				{
					quit = true;
				}
				else
					fsm.cycle(ev);
				delete ev;
			}
		} while (!quit);
		return 0;
	}
	else
	{
		cout << "ERROR INSTALANDO PARTE GRAFICA" << endl;
	}
    return 0;
}






/*************************************************************************************/
/*                                 MAIN DE FASE 1                                    */
/**************************************************************************************

Blockchain bchain;

uint myCounter = 0;
Graphic UserInterface(bchain);

if (!UserInterface.GetError())
{
    while (UserInterface.Running())
    {
        if (UserInterface.inputReady()) { //Si hay nuevo input lo analizamos


            UserInterface.processingRequest();

            //--------//
            // EN PROXIMAS ACA ETAPAS HAREMOS ALGO 
            // UNA VEZ QUE SE TEMRINA DE HACER SE LLAMA A LA FUNCION success()
            //--------//

            UserInterface.success();
        }
    }
}
else {
    cout << "Error, couldn't initialize Gui object!" << endl;
}
***************************************************************************************
***************************************************************************************/
