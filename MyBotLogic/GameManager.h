#pragma once

#include "LevelInfo.h"
#include "Graph.h"
#include "Agent.h"
#include "Utility.h"
#include <vector>
#include "Debug.h"

#ifdef DEBUG
class DuplicateObjectiveException;
#endif // DEBUG
class GameManager {
private:
	static GameManager instance;

	Graph graph;

	class AgentPtrComparison {
	private:
		bool reverse=false;
	public:
		AgentPtrComparison(const bool& revparam = false) : reverse{ revparam } {}
		bool operator() (Agent* _lagent, Agent* _ragent) const
		{
			if (reverse)
				return (_lagent->getPath().size() > _ragent->getPath().size());
			else
				return (_lagent->getPath().size() < _ragent->getPath().size());
		}
	};

	vector<Agent*> agents = {};


	GameManager();
	GameManager(GameManager&) = delete;
	GameManager& operator=(GameManager&) = delete;
	~GameManager();


public:
	static GameManager& get() {
		return instance;
	}

	std::vector<Agent*>::iterator getBeginAgent() noexcept {
		return agents.begin();
	}
	std::vector<Agent*>::iterator getEndAgent() noexcept {
		return agents.end();
	}

	std::vector<Agent*>& getAgents() noexcept {
		return agents;
	}

	Agent* getAgentById(int agentId) noexcept {
		auto agentIt = find_if(agents.begin(), agents.end(), [&agentId](Agent* agent) {
			return agent->getId() == agentId; 
		});
		return *agentIt;
	}

	Graph& getGraph() {
		return graph;
	}

	void start(LevelInfo& _levelInfo);
	void update(TurnInfo& _turnInfo, std::vector<Action*>& _actionList);
};
