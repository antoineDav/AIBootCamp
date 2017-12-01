#include "MoveState.h"
#include "../Agent.h"
#include "../MyBotLogic.h"
#include "../GameManager.h"
#include "../LogicManager.h"

#include "WaitState.h"
#include "../MissionManager.h"
//#include <chrono>

MoveState::MoveState()
{
}

State * MoveState::getTransition(TurnInfo & _turnInfo, Agent * agent)
{	
	Graph& graph = GameManager::get().getGraph();
	bool found = false;
	for_each(GameManager::get().getBeginAgent(), GameManager::get().getEndAgent(), [&](Agent * ag) {
		// Determine si un autre agent va tenter de se déplacer sur la même position que notre agent (CONFLIT) 
		if (ag->getId() != agent->getId() && ag->getNextPos() == agent->getNextPos() 
			&& (graph.dist(agent->getPos(), agent->getGoal()) < graph.dist(ag->getPos(), ag->getGoal()) 
				|| (graph.dist(agent->getPos(), agent->getGoal()) == graph.dist(ag->getPos(), ag->getGoal()) && agent->getId() > ag->getId()))) {
			found = true;
		}
	});

	if (found || agent->getPos() == agent->getGoal() || agent->getHasToWait()) {
		for_each(GameManager::get().getBeginAgent(), GameManager::get().getEndAgent(), [&](Agent * ag) {
			if (ag->getNextPos() == agent->getPos()/* && (ag->getCurrState() == &LogicManager::get().getWaitState() || ag->getPath().empty())*/)
			{
				ag->forceToWait(agent);
			}
		});
		return &LogicManager::get().getWaitState();
	}

	// Détermine si l'agent se trouve devant une porte avec plaque de pression distante
	const Connector* co = agent->getPath().back();
	if (co->hasDoor()) {
		std::set<Object::EObjectState> objectStates = graph.getObjects()[co->getObjects()].objectStates;
		if (!GameManager::get().getGraph().getObjects()[co->getObjects()].connectedTo.empty()
			&& objectStates.find(Object::ObjectState_Opened) == objectStates.end())
		{
			MissionManager& mm = MissionManager::get();
			
			mm.requestMission(agent->getId(), co->getObjects(), co->getEndNode()->getId());
			//mm.createGoalMission(co->getEndNode()->getId());

			return &LogicManager::get().getWaitCoopState(); 
		}
		return nullptr;
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
		std::set<Object::EObjectState> objectStateList = GameManager::get().getGraph().getObjects()[objectInfoId].objectStates;
		auto doorState = objectStateList.find(Object::ObjectState_Closed);
		if (doorState != objectStateList.end() && GameManager::get().getGraph().getObjects()[co->getObjects()].connectedTo.empty()) {
			return new Interact(agent->getId(), GameManager::get().getGraph().getObjects()[objectInfoId].objectID, Interact::Interaction_OpenDoor);
		}
	}

	ObjectInfo obj = GameManager::get().getGraph().getObjects()[co->getObjects()];
	if (find(obj.objectTypes.begin(), obj.objectTypes.end(), Object::ObjectType_Wall) != obj.objectTypes.end()) {
		GameManager::get().getGraph().setWallGrope(const_cast<Connector*>(co));
		return new Interact(agent->getId(), obj.objectID, Interact::Interaction_SearchHiddenDoor);
	}

	agent->setPos(agent->getNextPos());
	Action * act = new Move(agent->getId(), agent->getPath().back()->getDirection());
	agent->getPath().pop_back();
	return act;
}

void MoveState::onExit(Agent * agent)
{
}
