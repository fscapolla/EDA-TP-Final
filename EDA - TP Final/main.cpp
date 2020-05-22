#include <iostream>
#include "json.hpp"
#include "Block.h"
#include "Spv.h"
#include "Graphic.h"
#include "Blockchain.h"
#include <ios>

int main(void)
{

    Blockchain bchain;

    uint myCounter = 0;
    Graphic UserInterface(bchain);



    if (!UserInterface.GetError())
    {
        while (UserInterface.RunningOne())
        {
            if (UserInterface.inputReady()) { //Si hay nuevo input lo analizamos
                

                UserInterface.processingRequest();  // Avisar a la gui que vamos a arrancar a procesar las imgs

                bool allOk = false;
                ++myCounter;


                //--------//
                //   HAY Q CONECTAR BCHAIN CON RETURN JDATA
                //--------//

                UserInterface.success();
                /*
                if (CONDICION)
                {
                    UserInterface.success();
                }
                */
            }
        }
    }
    else {
        cout << "Error, couldn't initialize Gui object!" << endl;
    }

    return 0;
}