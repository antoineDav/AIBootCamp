#include "WaitState.h"
#include "../Agent.h"
#include "../MyBotLogic.h"
#include "../GameManager.h"
#include "../LogicManager.h"
#include <algorithm>
#include "MoveState.h"
#include "../MissionManager.h"

WaitCoopState::WaitCoopState()
{
}

State * WaitCoopState::getTransition(TurnInfo & _turnInfo, Agent * agent)
{
	
	const Connector* co = agent->getPath().back();
	std::set<Object::EObjectState> objectStates = GameManager::get().getGraph().getObjects()[co->getObjects()].objectStates;
	std::set<Object::EObjectType> objectTypes = GameManager::get().getGraph().getObjects()[co->getObjects()].objectTypes;
	// Vérifier si la mission de coopération a été accomplis ou si le path a changé
	if (objectStates.find(Object::ObjectState_Opened) != objectStates.end() || objectTypes.find(Object::ObjectType_Door) == objectTypes.end())
	{
		return &LogicManager::get().getMoveState();
	}
	
	// Vérifier si la coopération est impossible

	return nullptr;
}

void WaitCoopState::onEnter(Agent * agent)
{
}

Action * WaitCoopState::onUpdate(TurnInfo& _turnInfo, Agent * agent)
{
	return new Move(agent->getId(), Tile::CENTER);
}

void WaitCoopState::onExit(Agent * agent)
{
	Graph& graphe = GameManager::get().getGraph();
	const Connector* co = agent->getPath().back();

	// In case the agent is alone and can't cooperate
	std::set<Object::EObjectState> objectStates = graphe.getObjects()[co->getObjects()].objectStates;
	if (!GameManager::get().getGraph().getObjects()[co->getObjects()].connectedTo.empty()
		&& objectStates.find(Object::ObjectState_Opened) == objectStates.end())
	{
		// Invalid the connector
		Connector* co2 = const_cast<Connector* >(co);
		graphe.addForbiddenConnector(co2);
	}
}

char* WaitCoopState::stringType()
{
	return "WaitCoopState";
}
