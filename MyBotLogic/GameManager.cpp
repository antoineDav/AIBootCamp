#include "State.h"
#include "GameManager.h"

#include "windows.h"
#include "MissionManager.h"

#include "Debug.h"

GameManager GameManager::instance;

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
	for_each(getBeginAgent(), getEndAgent(), [](Agent* ag) { delete(ag); });
}


void GameManager::start(LevelInfo & _levelInfo)
{
	graph.init(_levelInfo.rowCount, _levelInfo.colCount, _levelInfo.tiles, _levelInfo.objects);
	for (const auto& npc : _levelInfo.npcs)
	{
		Agent * ag = new Agent((int)npc.second.npcID );
		ag->setPos(npc.second.tileID);
		agents.push_back(ag);
	}
}

void GameManager::update(TurnInfo & _turnInfo, std::vector<Action*>& _actionList)
{
	graph.update(_turnInfo.tiles,_turnInfo.objects);
	for (Agent * agent : agents) {
		agent->setVisited();
		agent->checkPath();
		agent->setHasToWait(false);
	}
	graph.popInvalidConnectors();

#ifdef DEBUGBOT_GRAPH
	
#endif // DEBUGBOT_GRAPH

	sort(agents.begin(), agents.end(), AgentPtrComparison());

	MissionManager::get().update();

	for (Agent * agent : agents)
	{
		agent->makeDecisions();
	}
	for (Agent * agent : agents)
	{
		agent->stateChange(_turnInfo);
	}
	for (Agent * agent : agents)
	{
		_actionList.push_back(agent->play(_turnInfo));
	}
	//MissionManager::get().newGoalFound = false;
}
