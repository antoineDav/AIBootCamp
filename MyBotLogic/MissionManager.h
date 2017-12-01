#include "MyBotLogic\Mission.h"
#include "MyBotLogic\Agent.h"

#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include "Logger.h"

using namespace std;

class MissionManager {

public:

	using MissionPtr = shared_ptr<Mission>;
	int tour;
private:
	Logger mLog;
	

	static MissionManager instance;

	vector<MissionPtr> missions; 

	int IdGenerator;
	MissionManager() : IdGenerator{}, missions{}, tour{} {
		//mLog.Init("\\INF781\\Labos\\AIBootCamp_3\\AIBot\\LocalMatchResults\\gamelog", "missions.txt");
		mLog.Init("C:\\Users\\patn2904\\Documents\\INF781_IA\\AIBootCamp\\AIBot\\LocalMatchResults\\gamelog", "missions.txt");
	}
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

	const vector<MissionPtr>& getMissions() {
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

	MissionPtr getCurrentlyAssignedMission (int npcId) {
		auto ptr = find_if(missions.begin(), missions.end(),
			[&npcId](MissionPtr ptr) {
			return (ptr->receiverId == npcId) && (ptr->mStatus != Mission::ACCOMPLISHED);
		}
		);

		if (ptr != missions.end()) {
			return *ptr;
		}
		else {
			return nullptr;
		}
	}

	MissionPtr getCurrentMissionGivenBy(int npcId) {
		auto ptr = find_if(missions.begin(), missions.end(),
			[&npcId](MissionPtr ptr) {
			return (ptr->giverId == npcId) && (ptr->mStatus == Mission::IN_PROGRESS);
		}
		);

		if (ptr != missions.end()) {
			return *ptr;
		}
		else {
			return nullptr;
		}
	}

	MissionPtr getMissionToTile(int tId) {
		auto ptr = find_if(missions.begin(), missions.end(),
			[&tId](MissionPtr ptr) {
			return (ptr->tileId == tId) && (ptr->mStatus == Mission::AVAILABLE);
		}
		);

		if (ptr != missions.end()) {
			return *ptr;
		}
		else {
			return nullptr;
		}
	}



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

	MissionPtr createGoalMission(int tileId) {
		if (!missionExists(-1, tileId)) {
			MissionPtr mission = make_shared<Mission>(getNewId(), -1, 1, Mission::AVAILABLE, -1, tileId);
			missions.push_back(mission);
			newGoalFound = true;

			return mission;
		}

		return nullptr;
	}

	MissionPtr createCoopMission(int giverId, int ppId, int tileId, int condId) {

		if (!missionExists(giverId, tileId))
		{
			MissionPtr ptr = getCurrentlyAssignedMission(giverId);
			int pLvl = ptr != nullptr ? ptr->priorityLvl + 1 : 2;
	
	
			Mission::missionStatus status = tileId == -1 ? Mission::PENDING : Mission::AVAILABLE;
			MissionPtr mission = make_shared</*Coop*/Mission>(getNewId(), giverId, pLvl, status, condId, tileId, ppId);
	
			missions.push_back(mission);
	
			newGoalFound = true; //A REVOIR
	
			return mission;
		}

		return nullptr;
	}

	bool missionExists(int gId, int tId) {
		auto it = find_if(missions.begin(), missions.end(), [&gId, &tId](MissionPtr mission) {
			return mission->giverId == gId && mission->tileId == tId;
		});

		return it != missions.end();
	}

	void swapMissions(Agent& agGoal, Agent& otherAg);
public:

	void goalFound() {
		newGoalFound = true;
	}
	bool isGoalFound() {
		return newGoalFound;
	}
	
	int getNewId() { return ++IdGenerator; }

	void requestMission(int npcId, int pressurePlateId, int conditionTile);
	bool assignMission(MissionPtr newMission, MissionPtr currentMission, Agent* agent);

	void update();

	int getBestAgent(MissionPtr&);

	void updatePendingMission(MissionPtr&);

	void updateInProgressMission(MissionPtr& mission);
};
