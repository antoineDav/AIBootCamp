#include "MissionManager.h"
#include "MyBotLogic\Graph.h"
#include "MyBotLogic\GameManager.h"

MissionManager MissionManager::instance;

void MissionManager::missionDone(int npcId, int missionId) {
	//change isHelping in agent
	for_each(GameManager::get().getBeginAgent(), GameManager::get().getEndAgent(), [&](Agent * ag) {
		if (ag->getId() == npcId) {
			ag->setHelping(false);
		}
	});
	inProgressCoopMissions.erase(missionId);
}

void MissionManager::requestMission(int npcId, int doorId) {
	Graph& graph = GameManager::get().getGraph();

	auto pps = graph.getObjects()[doorId].connectedTo;

	//for (auto& ppId : graph.getObjects()[doorId].connectedTo) {

	for_each(pps.begin(), pps.end(), [&](int ppId) {

		int ppTileId = graph.getPressurePlatePosition(ppId);

		if (ppTileId != -1) {
			createCoopMission(npcId, ppId, ppTileId);
		}
		else {
			createPendingCoopMission(npcId, ppId);
		}
	});

}

void MissionManager::update() {

	GameManager& gm = GameManager::get();

	updatePendingMissions();

	for (Agent* agent : gm.getAgents()) {

		if (!agent->getIsHelping()) {

			int currentMissionId = agent->getMissionId();

			//Check if coop missions are available
			if (!availableCoopMissions.empty()) {
				
				int newMissionId = getBestMission(availableCoopMissions, agent);
	
				if (newMissionId != -1) { //If coop mission assigned
	
					if (currentMissionId != -1) {
						returnGoalMission(currentMissionId);
					}
	
					agent->setHelping(true);
					takeCoopMission(newMissionId, agent->getId());
				}

			}
			else if (!agent->hasReachedGoal()) { //Check if goal missions are available

				if (!availableGoalMissions.empty() && currentMissionId == -1
					|| newGoalFound && currentMissionId != -1) {

					returnGoalMission(currentMissionId);

					int newMissionId = getBestMission(availableGoalMissions, agent);

					if (newMissionId != -1) { //If goal mission assigned
						agent->setHelping(false);
						takeGoalMission(newMissionId);
					}
				}
			}

		}
	}
	newGoalFound = false;
}

void MissionManager::updatePendingMissions() {

	Graph& graph = GameManager::get().getGraph();

	for (auto& mission : pendingCoopMissions) {

		int ppId = mission.second->pressurePlateId;

		int ppTileId = graph.getPressurePlatePosition(ppId);

		//If pressure plate location was discovered, update and move to available missions
		if (ppTileId != -1) {
			mission.second->tileId = ppTileId;
			availableCoopMissions[mission.first] = mission.second;
			pendingCoopMissions.erase(mission.first);
		}
	}
}

template <class MPtr>
int MissionManager::getBestMission(map<unsigned int, MPtr>& missions, Agent* agent) {
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
				agent->setPathValid(true);
				agent->setSearching(false);
				agent->setMissionId(minMissionId, minId);
				return minMissionId;
			}
		}
	}
	
}

