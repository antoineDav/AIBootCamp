#include "Agent.h"
#include "State/MoveState.h"
#include "State/WaitState.h"
#include "LogicManager.h"
#include "GameManager.h"
#include "Graph.h"

#ifdef DEBUGBOT
#include <fstream>
using namespace std;
static ofstream file{ "sortie.txt", ios::app };
#endif // DEBUGBOT

Agent::Agent(int agentId)
{
	id = agentId;
	missionId = -1;
	goalId = -1;
	pos = -1;
	currState = &LogicManager::get().getMoveState();
	isSearching = true;
	isHelping = false;
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
#ifdef DEBUGBOT_TREE
	file << "\tId :  " << std::to_string(this->getId()) << " - pos : " << std::to_string(this->getPos()) << " - " << currState->stringType() << endl;
#endif DEBUGBOT_TREE
}

void Agent::checkPath()
{
	for (const Connector * con: path) {
		if (con->getIsToDestroy()) {
			pathValid = false;
		}
	}
}

void Agent::forceToWait(Agent * source) {
	setCurrState(&LogicManager::get().getWaitState());
	setHasToWait(true);
	for (auto ag : GameManager::get().getAgents()) {
		if (ag->getId() != id && ag->getNextPos() == getPos()) {
			ag->forceToWait(this);
		}
	}
}

void Agent::setVisited() {
	GameManager::get().getGraph().setVisitedNode(this->pos);
}

