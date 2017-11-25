#pragma once
#include "../State.h"
#include "TurnInfo.h"

class WaitState : public State{
public:
	WaitState();

	State * getTransition(TurnInfo& _turnInfo, Agent * agent);
	void onEnter(Agent * agent);
	Action * onUpdate(TurnInfo& _turnInfo, Agent * agent);
	void onExit(Agent * agent);
};