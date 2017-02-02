#ifndef STATEEXPLORE_H

#include "..\FSMState.h"
#include "..\utility.h"

// Looks for powerup.
class StateExplore : public FSMState
{
public:
	StateExplore(Control* parent) :FSMState(FSM_STATE_EXPLORE, parent) {}
	~StateExplore();
	void Update(float dt);
	int CheckTransitions();

private:
	void NewRandomPos();
	Point3f m_randPos;
};

#endif