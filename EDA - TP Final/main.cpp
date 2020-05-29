#include <iostream>
#include "json.hpp"
#include "Block.h"
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

    return 0;
}