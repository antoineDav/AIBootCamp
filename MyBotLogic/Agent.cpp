#include "Agent.h"
#include "State/MoveState.h"
#include "State/WaitState.h"
#include "LogicManager.h"
#include "GameManager.h"

Agent::Agent(int agentId)
{
	id = agentId;
	currState = &LogicManager::get().getMoveState();
	isSearching = true;
}

void Agent::makeDecisions()
{
	LogicManager::get().getDecisionTree().execute(this);
}

Action * Agent::play(TurnInfo& _turnInfo)
{
	return currState->onUpdate(_turnInfo, this);
}

void Agent::stateChange(TurnInfo& _turnInfo)
{
	State * trans;
	trans = currState->getTransition(_turnInfo, this);
	int count = 1000;
	while (trans != nullptr && count>0) {
		currState->onExit(this);
		currState = trans;
		currState->onEnter(this);
		count--;
		trans = currState->getTransition(_turnInfo, this);
	}
}

void Agent::checkPath()
{
	for (const Connector * con: path) {
		if (con->getIsToDestroy()) {
			pathValid = false;
		}
	}
}


