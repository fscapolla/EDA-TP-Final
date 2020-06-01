#pragma once

#include "eventHandling.h"

class genericFSM;
using stateTypes = unsigned int;

struct fsmCell
{
	stateTypes nextState;
	void (genericFSM::* action)(genericEvent*);
};


class genericFSM : public eventGenerator
{
public:

	genericFSM(const fsmCell* const table, const unsigned int rows, const unsigned int columns, stateTypes initState);
	genericEvent* getEvent(unsigned int evento);
	void cycle(genericEvent* ev);

protected:
	stateTypes state;

private:


	genericEvent* fsmEvent = nullptr;

	const unsigned int rowCount;
	const unsigned int columnCount;
	const fsmCell* const FSMTable;

};
