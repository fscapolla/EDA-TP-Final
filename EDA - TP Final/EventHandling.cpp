#include "eventHandling.h"

using namespace std;


genericEvent* mainEventGenerator::
getNextEvent(unsigned int estado)
{
	//Por cada getNextEvent primero me fijo en cada event generator
	//si tiene un nuevo evento  para asegurarme de no perder ninguno.
	//siempre se sigue la premisa de que la velocidad del dispatcher
	//en este caso la FSM es mucho más rápida que la frecuencia de eventos
	//por lo que basta con revisar una vez cada fuente de eventos.
	//si no fuera el caso se debería iterar n veces por todos los generadores
	//hasta que ninguno tenga más nada que reportar.
	for (eventGenerator* evg : generators)
	{
		genericEvent* temp = evg->getEvent(estado);
		eventQueue.push(temp);
	}


	genericEvent* ret = nullptr;

	//If queue is not empty, saves firts item and pops it out of the queue.
	//Then returns first item or NULL if the queue is empty.
	if (!eventQueue.empty())
	{
		ret = eventQueue.front();
		eventQueue.pop();
	}
	return ret;
}

void mainEventGenerator::
attach(eventGenerator* evg)
{
	generators.push_back(evg);
}

void mainEventGenerator::
detach(eventGenerator* evg)
{
	generators.remove(evg);
}
