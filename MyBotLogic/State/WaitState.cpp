#include "WaitState.h"
#include "../Agent.h"
#include "../MyBotLogic.h"
#include "../GameManager.h"
#include "../LogicManager.h"
#include <algorithm>
#include "MoveState.h"

WaitState::WaitState()
{
}

State * WaitState::getTransition(TurnInfo & _turnInfo, Agent * agent)
{
	Graph graph = GameManager::get().getGraph();
	bool found = false;
	for_each(GameManager::get().getBeginAgent(), GameManager::get().getEndAgent(), [&](Agent * ag) {
		if (ag->getId() != agent->getId() && ag->getNextPos() == agent->getNextPos() && (graph.dist(agent->getPos(), agent->getGoal()) < graph.dist(ag->getPos(), ag->getGoal()) || (graph.dist(agent->getPos(), agent->getGoal()) == graph.dist(ag->getPos(), ag->getGoal()) && agent->getId() > ag->getId()))) {
			found = true;
		}
	});
	if (found || agent->getPos() == agent->getGoal()) {
		return nullptr;
	}
	else {
		return &LogicManager::get().getMoveState();
	}
}

void WaitState::onEnter(Agent * agent)
{
}

Action * WaitState::onUpdate(TurnInfo& _turnInfo, Agent * agent)
{
	return new Move(agent->getId(), Tile::CENTER);
}

void WaitState::onExit(Agent * agent)
{
}
