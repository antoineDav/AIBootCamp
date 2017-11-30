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
	int missionId;
	int goalId;
	int pos;
	vector<const Connector *> path;
	vector<int> forbidden;
	State * currState;

	bool pathValid = true;
	bool isSearching = false;
	bool isHelping = false;
	bool hasToWait = false;
	bool doNotDisturb = false;

public:
	int getId() {
		return id;
	}
	int getMissionId() {
		return missionId;
	}
	void setMissionId(int nMission, int nGoal) {
		missionId = nMission;
		goalId = nGoal;

	}
	int getGoal() {
		return goalId;
	}
	void setGoal(int nGoal) {
		goalId = nGoal;
	}
	int getPos() {
		return pos;
	}
	void setPos(int nPos) {
		pos = nPos;
	}

	bool hasReachedGoal() {
		return pos == goalId;
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
	void setHelping(bool b) {
		isHelping = b;
	}
	bool getIsHelping() {
		return isHelping;
	}

	void setDoNotDisturb(bool b) {
		doNotDisturb = b;
	}
	bool getDoNotDisturb() {
		return doNotDisturb;
	}

	State* getCurrState() { return currState; }
	void setCurrState(State * newState) { currState = newState; }
	bool getHasToWait() { return hasToWait; }
	void setHasToWait(bool newBool) { hasToWait = newBool; }
	void forceToWait(Agent * source);
};