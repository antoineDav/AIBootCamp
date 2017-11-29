#include "FindObjective.h"
#include "../Agent.h"
#include "../GameManager.h"

//Task::Result FindObjective::execute(Agent * agent)
//{
//	//GameManager& gm = GameManager::get();
//	//MissionManager& mm = MissionManager::get();
//	//vector<int>& forbiddens = agent->getForbiddens();
//	//if (!agent->getIsSearching()) {
//	//	mm.returnGoalMission(agent->getGoal());
//	//}
//
//
//
//
//	//if (findClosestMission(mm.getAvailableCoopMissions(), agent) || findClosestMission(mm.getAvailableGoalMissions(), agent)) {
//
//	//	agent->setPathValid(true);
//	//	agent->setSearching(false);
//	//	return Task::SUCCESS;
//
//	//}
//	//else {
//	//	return Task::FAILURE;
//	//}
//	//
//
//	////bool found = false;
//	////while (!found) {
//	////	int minDist = -1;
//	////	int minId = -1;
//	////	for (int goal : objectives) {
//	////		if (std::find(forbiddens.begin(), forbiddens.end(), goal) == forbiddens.end()) {
//	////			int dist = gm.getGraph().dist(agent->getPos(), goal);
//	////			if (minDist == -1 || dist < minDist) {
//	////				minDist = dist;
//	////				minId = goal;
//	////			}
//	////		}
//	////	}
//	////	if (minId == -1) {
//	////		return Task::FAILURE;
//	////	}
//	////	else {
//	////		vector<const Connector *> path = gm.getGraph().getPath(agent->getPos(), minId);
//	////		if (path.size() == 0) {
//	////			agent->addForbidden(minId);
//	////		}
//	////		else {
//	////			found = true;
//	////			agent->setPath(path);
//	////			mm.takeObjective(minId);
//	////			agent->setGoal(minId);
//	////		}
//	////	}
//	////}
//
//
//}

//bool FindObjective::findClosestMission(const vector<Mission>& missions, Agent* agent) {
//
//	GameManager& gm = GameManager::get();
//	MissionManager& mm = MissionManager::get();
//
//	vector<int>& forbiddens = agent->getForbiddens();
//	bool found = false;
//
//	while (!found) {
//		int minDist = -1;
//		int minId = -1;
//		int minMissionId = -1;
//		for (Mission m : missions) {
//			if (std::find(forbiddens.begin(), forbiddens.end(), m.tileId) == forbiddens.end()) {
//				int dist = gm.getGraph().dist(agent->getPos(), m.tileId);
//				if (minDist == -1 || dist < minDist) {
//					minDist = dist;
//					minId = m.tileId;
//					minMissionId = m.missionId;
//				}
//			}
//		}
//		if (minId == -1) {
//			return false;
//		}
//		else {
//			vector<const Connector *> path = gm.getGraph().getPath(agent->getPos(), minId);
//			if (path.size() == 0) {
//				agent->addForbidden(minId);
//			}
//			else {
//				found = true;
//				agent->setPath(path);
//				mm.takeCoopMission(minMissionId);
//				agent->setGoal(minId);
//				return true;
//			}
//		}
//	}
//}
