#pragma once
#include <list>
#include <queue>

//enunm Forward Declaration
//it is feasible as long as the type is defined in the code.
using eventTypes = unsigned int;

class genericEvent
{
public:
	virtual eventTypes getType(void) = 0;
	bool operator==(eventTypes ev) { return (this->getType() == ev); } //Easy Compare an Event With its types.
	virtual bool const operator!() const { return  false; } 		        //This operator can be used to understand if the event is present or not. ie: No Event.
};

class eventGenerator
{
public:
	virtual genericEvent* getEvent(unsigned int estado) = 0;  // Returns NULL if there's no new Event or a pointer to a concrete instance in the heap of a genericEvent if there's a new event
													   // User must then free this instance.
};

class mainEventGenerator
{
public:
	genericEvent* getNextEvent(unsigned int estado);
	void attach(eventGenerator* evg);
	void detach(eventGenerator* evg);

private:
	std::queue<genericEvent*>  eventQueue; //It can also be a priority_queue if events have got some sort of priority
	std::list<eventGenerator*> generators; //It will contain all eventGenerators attached to the mainEventGenerator
};
