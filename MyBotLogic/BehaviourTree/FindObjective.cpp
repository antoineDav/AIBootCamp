#include "FindObjective.h"
#include "../Agent.h"
#include "../GameManager.h"

Task::Result FindObjective::execute(Agent * agent)
{
	GameManager& gm = GameManager::get();
	vector<int>& forbiddens = agent->getForbiddens();
	if (!agent->getIsSearching()) {
		gm.returnObjective(agent->getGoal());
	}
	vector<int> objectives = gm.getAvailableObjectives();
	bool found = false;
	while (!found) {
		int minDist = -1;
		int minId = -1;
		for (int goal : objectives) {
			if (std::find(forbiddens.begin(), forbiddens.end(), goal) == forbiddens.end()) {
				int dist = gm.getGraph().dist(agent->getPos(), goal);
				if (minDist == -1 || dist < minDist) {
					minDist = dist;
					minId = goal;
				}
			}
		}
		if (minId == -1) {
			return Task::FAILURE;
		}
		else {
			vector<const Connector *> path = gm.getGraph().getPath(agent->getPos(), minId);
			if (path.size() == 0) {
				agent->addForbidden(minId);
			}
			else {
				found = true;
				agent->setPath(path);
				gm.takeObjective(minId);
				agent->setGoal(minId);
			}
		}
	}
	agent->setPathValid(true);
	agent->setSearching(false);
	return Task::SUCCESS;
}
