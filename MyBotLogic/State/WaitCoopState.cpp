#include "WaitState.h"
#include "../Agent.h"
#include "../MyBotLogic.h"
#include "../GameManager.h"
#include "../LogicManager.h"
#include <algorithm>
#include "MoveState.h"

WaitCoopState::WaitCoopState()
{
	wclog.Init("C:\\Users\\dava3202\\Documents\\Cours\\IA\\CD_AIBootCamp\\AIBootCamp\\MyBotLogic", "wclog.txt");
}

State * WaitCoopState::getTransition(TurnInfo & _turnInfo, Agent * agent)
{
	// Vérifier si la mission de coopération a été accomplis
	if (true/*Objectif n'apparait plus dans la liste*/)
	{
		return &LogicManager::get().getMoveState();
	}
	return nullptr;
}

void WaitCoopState::onEnter(Agent * agent)
{
	// Envoyer requete de coopération
	wclog.Log("		WaitCoopState onEnter");
	wclog.Log("			Requette coop");

}

Action * WaitCoopState::onUpdate(TurnInfo& _turnInfo, Agent * agent)
{
	return new Move(agent->getId(), Tile::CENTER);
}

void WaitCoopState::onExit(Agent * agent)
{
	// In case the agent is alone and can't cooperate
	Graph graphe = GameManager::get().getGraph();
	const Connector* co = agent->getPath().back();
	std::set<Object::EObjectState> objectStates = graphe.getObjects()[co->getObjects()].objectStates;
	if (!GameManager::get().getGraph().getObjects()[co->getObjects()].connectedTo.empty()
		&& objectStates.find(Object::ObjectState_Opened) == objectStates.end())
	{
		// Invalid the connector
/*		agent->setPathValid(false);*/
		Connector* co2 = const_cast<Connector* >(co);
		graphe.addForbiddenConnector(co2);
	}
}
