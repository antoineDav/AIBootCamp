#include "MissionManager.h"
#include "MyBotLogic\Graph.h"
#include "MyBotLogic\GameManager.h"

MissionManager MissionManager::instance;

void MissionManager::missionDone(int npcId, int missionId) {
	//TODO change isHelping in agent
	MissionPtr ptr = getMissionById(missionId);
	
	ptr->mStatus = Mission::ACCOMPLISHED;
}

void MissionManager::requestMission(int npcId, int doorId) {
	Graph& graph = GameManager::get().getGraph();

	auto pps = graph.getObjects()[doorId].connectedTo;

	for_each(pps.begin(), pps.end(), [&](int ppId) {
		createCoopMission(npcId, ppId, graph.getPressurePlatePosition(ppId));
	});

}

void MissionManager::update() {

	GameManager& gm = GameManager::get();

	//Update pending missions if possible
	for (auto& mission : missions) {

		if (mission->mStatus == Mission::PENDING) {
			updatePendingMission(mission);
		}
	}

	//Sort missions by priority
	sort(missions.begin(), missions.end(), MissionPtrComparison());


	//Try to assign available missions in order of priority
	for (auto& mission : missions) {

		if (mission->mStatus == Mission::AVAILABLE) {
			int agentId = getBestAgent(mission);
		}
	
	}
	
	//for (Agent* agent : gm.getAgents()) {
	//	if (!agent->getIsHelping()) {
	//		int currentMissionId = agent->getMissionId();
	//		//Check if coop missions are available
	//		if (!availableCoopMissions.empty()) {
	//			
	//			int newMissionId = getBestAgent(availableCoopMissions, agent);
	//
	//			if (newMissionId != -1) { //If coop mission assigned
	//
	//				if (currentMissionId != -1) {
	//					returnGoalMission(currentMissionId);
	//				}
	//
	//				agent->setHelping(true);
	//				takeCoopMission(newMissionId, agent->getId());
	//			}
	//		}
	//		else if (!agent->hasReachedGoal()) { //Check if goal missions are available
	//			if (!availableGoalMissions.empty() && currentMissionId == -1
	//				|| newGoalFound && currentMissionId != -1) {
	//				returnGoalMission(currentMissionId);
	//				int newMissionId = getBestAgent(availableGoalMissions, agent);
	//				if (newMissionId != -1) { //If goal mission assigned
	//					agent->setHelping(false);
	//					takeGoalMission(newMissionId);
	//				}
	//			}
	//		}
	//	}
	//}

	newGoalFound = false; //????
}

void MissionManager::updatePendingMission(MissionPtr& mission) {

	Graph& graph = GameManager::get().getGraph();

	int ppId = mission->pressurePlateId;

	int ppTileId = graph.getPressurePlatePosition(ppId);

	//If pressure plate location was discovered, update and move to available missions
	if (ppTileId != -1) {
		mission->tileId = ppTileId;
		mission->mStatus = Mission::AVAILABLE;
	}
}

int MissionManager::getBestAgent(MissionPtr& mission) {

	GameManager& gm = GameManager::get();

	bool found = false;
	int missionPriorityLvl = mission->priorityLvl;

	while (!found) {
		int minDist = -1;
		Agent* minNPC = nullptr;
		MissionPtr minNPCCurrentMission = nullptr;

		for (Agent* agent : gm.getAgents()) {

			//Check if agent already has a mission
			int currentPriorityLevel;
			MissionPtr ptr = getMissionGivenTo(agent->getId());

			if (ptr != nullptr) {
				currentPriorityLevel = ptr->priorityLvl;
			}
			else {
				currentPriorityLevel = -1;
			}


			if (agent->getId() != mission->giverId &&  //Agent cannot help itself
				missionPriorityLvl > currentPriorityLevel) //Mission must have higher priority to override previous mission
			{

				vector<int>& forbiddens = agent->getForbiddens();

				if (std::find(forbiddens.begin(), forbiddens.end(), mission->tileId) == forbiddens.end()) {

					int dist = gm.getGraph().dist(agent->getPos(), mission->tileId);
					if (minDist == -1 || dist < minDist) {
						minDist = dist;
						minNPC = agent;
						minNPCCurrentMission = ptr;
					}
				}
			}
		}

		if (minNPC == nullptr) {

			//Goal is forbidden for all agents
			mission->mStatus = Mission::IMPOSSIBLE;
			return -1;
			
		}
		else {
			//Check for valid path
			vector<const Connector *> path = gm.getGraph().getPath(minNPC->getPos(), mission->tileId);
			if (path.size() == 0) {
				minNPC->addForbidden(mission->tileId);
			}
			else {
				found = true;

				//Return current lower priority mission if any
				if (minNPCCurrentMission != nullptr) {
					returnMission(minNPCCurrentMission->missionId);
				}
				

				//Assign new mission
				minNPC->setPath(path);
				minNPC->setPathValid(true);
				minNPC->setSearching(false);
				minNPC->setMissionId(mission->missionId, mission->tileId);
				//Set ishelping?

				takeMission(mission->missionId, minNPC->getId());

				//TODO Update priorityLvl of agents helping this npc if any

				return minNPC->getId();
			}
		}
	}
}
