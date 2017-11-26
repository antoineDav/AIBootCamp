#pragma once
#include "../State.h"
#include "TurnInfo.h"
#include "Logger.h"

class WaitCoopState : public State {
public:
	WaitCoopState();

	Logger wclog;
	State * getTransition(TurnInfo& _turnInfo, Agent * agent);
	void onEnter(Agent * agent);
	Action * onUpdate(TurnInfo& _turnInfo, Agent * agent);
	void onExit(Agent * agent);
};