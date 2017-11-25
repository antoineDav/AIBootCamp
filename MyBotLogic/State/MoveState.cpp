#include "MoveState.h"
#include "../Agent.h"
#include "../MyBotLogic.h"
#include "../GameManager.h"
#include "../LogicManager.h"

#include "WaitState.h"

MoveState::MoveState()
{
}

State * MoveState::getTransition(TurnInfo & _turnInfo, Agent * agent)
{
	Graph graph = GameManager::get().getGraph();
	bool found = false;
	for_each(GameManager::get().getBeginAgent(), GameManager::get().getEndAgent(), [&](Agent * ag) {
		if (ag->getId() != agent->getId() && ag->getNextPos() == agent->getNextPos() 
			&& (graph.dist(agent->getPos(), agent->getGoal()) < graph.dist(ag->getPos(), ag->getGoal()) 
				|| (graph.dist(agent->getPos(), agent->getGoal()) == graph.dist(ag->getPos(), ag->getGoal()) && agent->getId() > ag->getId()))) {
			found = true;
		}
	});
	if (found || agent->getPos() == agent->getGoal()) {
		return &LogicManager::get().getWaitState();
	}
	else {
		return nullptr;
	}
}

void MoveState::onEnter(Agent * agent)
{
}

Action * MoveState::onUpdate(TurnInfo& _turnInfo, Agent * agent)
{
	const Connector* co = agent->getPath().back();
	if (co->hasDoor()) {
		int objectInfoId = co->getObjects();
		std::set<Object::EObjectType> objectTypeList = GameManager::get().getGraph().getObjects()[objectInfoId].objectTypes;
		std::set<Object::EObjectState> objectStateList = GameManager::get().getGraph().getObjects()[objectInfoId].objectStates;
		auto doorType = objectTypeList.find(Object::ObjectType_Door);
		auto doorState = objectStateList.find(Object::ObjectState_Closed);
		if (doorType != objectTypeList.end() && doorState != objectStateList.end()) {
			return new Interact(agent->getId(), GameManager::get().getGraph().getObjects()[objectInfoId].objectID, Interact::Interaction_OpenDoor);
		}
	}

	agent->setPos(agent->getNextPos());
	Action * act = new Move(agent->getId(), agent->getPath().back()->getDirection());
	agent->getPath().pop_back();
	return act;
}

void MoveState::onExit(Agent * agent)
{
}
