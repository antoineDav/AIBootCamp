#pragma once

#include "Globals.h"
#include "TurnInfo.h"

class Agent;
class State {
protected:
	State() = default;
private:
	State(State&) = delete;
	State& operator=(State&) = delete;
public:
	virtual State * getTransition(TurnInfo& _turnInfo, Agent * agent) = 0;
	virtual void onEnter(Agent * agent) = 0;
	virtual Action * onUpdate(TurnInfo& _turnInfo, Agent * agent) = 0;
	virtual void onExit(Agent * agent) = 0;
};