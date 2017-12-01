#include "MissionManager.h"
#include "MyBotLogic\Graph.h"
#include "MyBotLogic\GameManager.h"
#include <string>


MissionManager MissionManager::instance;

//void MissionManager::missionDone(int npcId, int missionId) {
//	//TODO change isHelping in agent
//	MissionPtr ptr = getMissionById(missionId);
//	
//	ptr->mStatus = Mission::ACCOMPLISHED;
//}

void MissionManager::requestMission(int npcId, int doorId, int conditionTile) {
	Graph& graph = GameManager::get().getGraph();

	auto pps = graph.getObjects()[doorId].connectedTo;

	for_each(pps.begin(), pps.end(), [&](int ppId) {
		createCoopMission(npcId, ppId, graph.getPressurePlatePosition(ppId), conditionTile);
	});

	//Create mission for giver to cross door
	MissionPtr mission = createGoalMission(conditionTile);
	mission->conditionTileId = conditionTile;
	mission->giverId = npcId;
	assignMission(mission, getCurrentlyAssignedMission(npcId), GameManager::get().getAgentById(npcId));

}




void MissionManager::update() {
	GameManager& gm = GameManager::get();


	//mLog.Log("Missions on turn " + to_string(gm.))
	mLog.Log("	-------------------NEW TURN!! " + std::to_string(++tour) +"------------------\n ");
	mLog.Log("\n\nAgents : \n");
	for (auto& agent : GameManager::get().getAgents())
	{
		mLog.Log("Id :  " + std::to_string(agent->getId()) + " - pos : " + std::to_string(agent->getPos()));
	}
	//Update pending missions if possible
	for (auto& mission : missions) {		
		mLog.Log("\nMission n " + std::to_string(mission->missionId));
		mLog.Log("	GiverId " + std::to_string(mission->giverId));
		mLog.Log("	ReceiverId " + std::to_string(mission->receiverId));
		mLog.Log("	tileId " + std::to_string(mission->tileId));
		mLog.Log("	conditionId " + std::to_string(mission->conditionTileId));
		mLog.Log("	priority " + std::to_string(mission->priorityLvl));
		mLog.Log("	status " + std::to_string(mission->mStatus));
		mLog.Log("	--------------------------------------------------\n ");

		if (mission->mStatus == Mission::PENDING) {
			updatePendingMission(mission);
		}
		else if (mission->conditionTileId != -1 && mission->mStatus == Mission::IN_PROGRESS) {
			updateInProgressMission(mission);
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

void MissionManager::updateInProgressMission(MissionPtr& mission) {

	//If the giver agent is on the condition tile, mission accompished
	GameManager& gm = GameManager::get();

	Agent* giverAgent = gm.getAgentById(mission->giverId);

	if (giverAgent->getPos() == mission->conditionTileId) {

		mission->mStatus = Mission::ACCOMPLISHED;

		Agent * receiverAgent = gm.getAgentById(mission->receiverId);
		
		//If receiver previously requested help, go back to that mission at all costs
		MissionPtr helpMission = getCurrentMissionGivenBy(mission->receiverId);

		if (helpMission != nullptr) {

			MissionPtr previousMission = getMissionToTile(helpMission->conditionTileId);

			assignMission(previousMission, nullptr, receiverAgent);

		}
		else { //Else, send back to explore mode

			receiverAgent->setSearching(true);
			receiverAgent->setMissionId(-1, -1);
			receiverAgent->setHelping(false);
		}

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
			MissionPtr ptr = getCurrentlyAssignedMission(agent->getId());

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
			//mission->mStatus = Mission::IMPOSSIBLE;
			return -1;
			
		}
		else {

			if (assignMission(mission, minNPCCurrentMission, minNPC)) {
				return minNPC->getId();
			}
			
		}
	}
}

bool MissionManager::assignMission(MissionPtr newMission, MissionPtr currentMission, Agent* agent) {

	GameManager& gm = GameManager::get();

	//Check for valid path
	vector<const Connector *> path = gm.getGraph().getPath(agent->getPos(), newMission->tileId);
	if (path.size() == 0) {
		agent->addForbidden(newMission->tileId);
		return false;
	}
	else {
		//Return current lower priority mission if any
		if (currentMission != nullptr) {
			returnMission(currentMission->missionId);
		}

		//Assign new mission
		agent->setPath(path);
		agent->setPathValid(true);
		agent->setSearching(false);
		agent->setMissionId(newMission->missionId, newMission->tileId);

		takeMission(newMission->missionId, agent->getId());

		return true;
	}
}
