#include <iostream>
#include <functional>
#include "genericFSM.h"

//Forward declarations..

using namespace std;
using namespace std::placeholders;

genericFSM::
genericFSM(const fsmCell* const table, const unsigned int rows, const unsigned int columns, stateTypes initState) :state(initState), rowCount(rows), columnCount(columns), FSMTable(table) {}

void genericFSM::
cycle(genericEvent* ev)
{
	if (ev != nullptr)
	{
		fsmCell temp = FSMTable[static_cast<unsigned int>(state) * columnCount + static_cast<unsigned int>(ev->getType())];
		auto f = bind(temp.action, this, ev);

		f();
		state = temp.nextState;
	}
	return;
}

genericEvent* genericFSM::
getEvent(unsigned int estado)
{
	return fsmEvent;
}