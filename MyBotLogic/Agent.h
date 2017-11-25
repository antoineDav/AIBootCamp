#pragma once
#include <vector>
#include "Globals.h"
#include "Connector.h"
#include "TurnInfo.h"
#include "State.h"
#include "Node.h"
#include "BehaviourTree/GoalTree.h"

using namespace std;

class Agent {
private:
	int id;
	int goal;
	int pos;
	vector<const Connector *> path;
	vector<int> forbidden;
	State * currState;

	bool pathValid;
	bool isSearching;

public:
	int getId() {
		return id;
	}
	int getGoal() {
		return goal;
	}
	void setGoal(int nGoal) {
		goal = nGoal;
	}
	int getPos() {
		return pos;
	}
	void setPos(int nPos) {
		pos = nPos;
	}

	void setPathValid(bool nPathValid) {
		pathValid = nPathValid;
	}
	bool getPathValid() {
		return pathValid;
	}

	void setPath(vector<const Connector *>& nPath) {
		path = nPath;
	}
	vector<const Connector *>& getPath() {
		return path;
	}
	int getNextPos() {
		if (path.size() > 0) {
			return path.back()->getEndNode()->getId();
		}
		else {
			return pos;
		}
	}
	vector<int>& getForbiddens() {
		return forbidden;
	}
	void addForbidden(int id) {
		forbidden.push_back(id);
	}
	Agent(int agentId);
	void makeDecisions();
	Action * play(TurnInfo& _turnInfo);
	void stateChange(TurnInfo& _turnInfo);
	void checkPath();
	void setSearching(bool b) {
		isSearching = b;
	}
	bool getIsSearching() {
		return isSearching;
	}
};