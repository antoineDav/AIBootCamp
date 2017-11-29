#pragma once
#include "Task.h"
#include <vector>

using namespace std;

class FindObjective : public Task {
	virtual Result execute(Agent *) override;

	//bool findClosestMission(const vector<Mission>& missions, Agent* agent);
};