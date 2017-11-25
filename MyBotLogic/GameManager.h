#pragma once

#include "LevelInfo.h"
#include "Graph.h"
#include "Agent.h"
#include "Utility.h"
#include <vector>

#ifdef DEBUG
class DuplicateObjectiveException;
#endif // DEBUG
class GameManager {
private:
	static GameManager instance;

	Graph graph;
	vector<int> availableObjectives;

	class AgentPtrComparison {
	private:
		bool reverse;
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

	vector<Agent*> agents;


	GameManager();
	GameManager(GameManager&) = delete;
	GameManager& operator=(GameManager&) = delete;
	~GameManager();

	bool newGoalFound;

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

	Graph& getGraph() {
		return graph;
	}
	const vector<int>& getAvailableObjectives() {
		return availableObjectives;
	}
	void takeObjective(int id) {
		availableObjectives.erase(std::find(availableObjectives.begin(), availableObjectives.end(), id));
	}
	void returnObjective(int id) {
#ifdef DEBUG
		if (std::find(availableObjectives.begin(), availableObjectives.end(), id) != availableObjectives.end()) {
			throw new DuplicateObjectiveException;
		}
#endif // DEBUG
		availableObjectives.push_back(id);
		newGoalFound = true;
	}
	void goalFound() {
		newGoalFound = true;
	}
	bool isGoalFound() {
		return newGoalFound;
	}
	void start(LevelInfo& _levelInfo);
	void update(TurnInfo& _turnInfo, std::vector<Action*>& _actionList);
};
