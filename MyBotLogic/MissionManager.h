#include "MyBotLogic\Mission.h"
#include "MyBotLogic\Agent.h"

#include <vector>
#include <map>
#include <memory>
#include <algorithm>

using namespace std;

class MissionManager {

public:

	using MissionPtr = shared_ptr<Mission>;
	//using CoopMissionPtr = shared_ptr<CoopMission>;

private:
	static MissionManager instance;

	vector<MissionPtr> missions; //FONCTION TRI PAR PRIORITÉ (haute à basse)

	//map<unsigned int, MissionPtr> availableGoalMissions;
	//map<unsigned int, CoopMissionPtr> availableCoopMissions;
	//
	//map<unsigned int, MissionPtr> inProgressGoalMissions;
	//map<unsigned int, CoopMissionPtr> inProgressCoopMissions;

	//map<unsigned int, CoopMissionPtr> pendingCoopMissions;

	int IdGenerator;
	MissionManager() : IdGenerator{}, missions{}/*, availableCoopMissions{}, availableGoalMissions{}*/ {}
	MissionManager(MissionManager&) = delete;
	MissionManager& operator=(MissionManager&) = delete;
	~MissionManager() = default;

	class MissionPtrComparison {
	public:
		MissionPtrComparison(){}

		bool operator() (MissionPtr _lmission, MissionPtr _rmission) const {
			return (_lmission->priorityLvl > _rmission->priorityLvl);
		}
	};

public:

	bool newGoalFound = false;
	static MissionManager& get() {
		return instance;
	}

	//const map<unsigned int, MissionPtr>& getAvailableGoalMissions() {
	//	return availableGoalMissions;
	//}
	//const map<unsigned int, CoopMissionPtr>& getAvailableCoopMissions() {
	//	return availableCoopMissions;
	//}

	const vector<MissionPtr> getMissions() {
		return missions;
	}

	
	MissionPtr getMissionById (int mId) {
		auto ptr =  find_if(missions.begin(), missions.end(), 
			[&mId](MissionPtr ptr) {
			return ptr->missionId == mId;
			}
		);

		if (ptr != missions.end()) {
			return *ptr;
		}
		else {
			return nullptr;
		}
		
	}

	MissionPtr getMissionGivenTo (int npcId) {
		auto ptr = find_if(missions.begin(), missions.end(),
			[&npcId](MissionPtr ptr) {
			return ptr->receiverId == npcId;
		}
		);

		if (ptr != missions.end()) {
			return *ptr;
		}
		else {
			return nullptr;
		}
	}

	//void takeCoopMission(int missionId, int rId) {
	//	inProgressCoopMissions[missionId] = availableCoopMissions[missionId];
	//	inProgressCoopMissions[missionId]->receiverId = rId;
	//	availableCoopMissions.erase(missionId);
	//}

	//void takeGoalMission(int missionId) {
	//	inProgressGoalMissions[missionId] = availableGoalMissions[missionId];
	//	availableGoalMissions.erase(missionId);
	//}

	void takeMission(int missionId, int rId = -1) {

		MissionPtr ptr = getMissionById(missionId);

		ptr->mStatus = Mission::IN_PROGRESS;
		ptr->receiverId = rId;

	}

	void returnMission(int missionId) {
#ifdef DEBUG
		if (std::find(availableGoals.begin(), availableGoals.end(), id) != availableGoals.end()) {
			throw new DuplicateObjectiveException;
		}
#endif // DEBUG
		if (missionId >= 0) {
			MissionPtr ptr = getMissionById(missionId);

			ptr->mStatus = Mission::AVAILABLE;
			ptr->receiverId = -1;

			newGoalFound = true; //?? A REVOIR
		}
		
	}

	//void returnCoopMission(int missionId) {
	//	availableCoopMissions[missionId] = inProgressCoopMissions[missionId];
	//	availableCoopMissions[missionId]->receiverId = -1;
	//	inProgressCoopMissions.erase(missionId);
	//}


	void createGoalMission(int tileId) {
		missions.push_back(make_shared<Mission>(getNewId(), -1, 1, Mission::AVAILABLE, tileId));
		newGoalFound = true;
	}

	void createCoopMission(int giverId, int ppId, int tileId) {

		MissionPtr ptr = getMissionGivenTo(giverId);
		int pLvl = ptr->priorityLvl + 1;

		if (tileId == -1) {
			missions.push_back(make_shared</*Coop*/Mission>(getNewId(), giverId, pLvl, Mission::PENDING, tileId, ppId));
		}
		else {
			missions.push_back(make_shared</*Coop*/Mission>(getNewId(), giverId, pLvl, Mission::AVAILABLE, tileId, ppId));
		}
		
		newGoalFound = true; //A REVOIR
	}

	/*void createPendingCoopMission(int giverId, int ppId) {
		int missionId = getNewId();
		availableCoopMissions[missionId] = make_shared<CoopMission>(missionId, giverId, ppId);
		newGoalFound = true;
	}*/

public:

	void goalFound() {
		newGoalFound = true;
	}
	bool isGoalFound() {
		return newGoalFound;
	}
	
	int getNewId() { return ++IdGenerator; }

	void missionDone(int npcId, int missionId);

	void requestMission(int npcId, int pressurePlateId);

	void update();

	int getBestAgent(MissionPtr&);

	void updatePendingMission(MissionPtr&);
};