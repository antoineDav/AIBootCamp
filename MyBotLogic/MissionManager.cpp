#include "MissionManager.h"
#include "MyBotLogic\Graph.h"
#include "MyBotLogic\GameManager.h"

MissionManager MissionManager::instance;

void MissionManager::missionDone(int npcId, int missionId) {
	//change isHelping in agent
}

void MissionManager::requestMission(int npcId, int pressurePlateId) {

}

void MissionManager::update() {

	GameManager& gm = GameManager::get();

	for (Agent* agent : gm.getAgents()) {

		if (!agent->getIsHelping()) {

			//Check if coop missions are available
			int newMissionId = getBestMission(availableCoopMissions, agent);

			if (newMissionId != -1) { //If coop mission assigned
				agent->setHelping(true);
				takeCoopMission(newMissionId);
			}
			else if (newGoalFound) { //Check if goal missions are available
				int currentMissionId = agent->getMissionId();

				if (currentMissionId != -1) {
					returnGoalMission(currentMissionId);
				}
				
				int newMissionId = getBestMission(availableGoalMissions, agent);
				agent->setHelping(false);
				takeGoalMission(newMissionId);
			}

		}
	}
}

int MissionManager::getBestMission(map<unsigned int, MissionPtr>& missions, Agent* agent) {
	GameManager& gm = GameManager::get();

	vector<int>& forbiddens = agent->getForbiddens();
	bool found = false;

	while (!found) {
		int minDist = -1;
		int minId = -1;
		int minMissionId = -1;
		for (auto& mission : missions) {
			if (std::find(forbiddens.begin(), forbiddens.end(), mission.second->tileId) == forbiddens.end()) {
				int dist = gm.getGraph().dist(agent->getPos(), mission.second->tileId);
				if (minDist == -1 || dist < minDist) {
					minDist = dist;
					minId = mission.second->tileId;
					minMissionId = mission.first;
				}
			}
		}
		if (minId == -1) {
			return -1;
		}
		else {
			vector<const Connector *> path = gm.getGraph().getPath(agent->getPos(), minId);
			if (path.size() == 0) {
				agent->addForbidden(minId);
			}
			else {
				found = true;
				agent->setPath(path);
				agent->setSearching(false);
				agent->setMissionId(minMissionId, minId);
				return minMissionId;
			}
		}
	}
	
}

