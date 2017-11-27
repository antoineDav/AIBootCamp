#include "MyBotLogic\Mission.h"
#include "MyBotLogic\Agent.h"

#include <vector>
#include <map>
#include <memory>

using namespace std;

class MissionManager {

public:

	using MissionPtr = shared_ptr<Mission>;
	using CoopMissionPtr = shared_ptr<CoopMission>;

private:
	static MissionManager instance;

	map<unsigned int, MissionPtr> availableGoalMissions;
	map<unsigned int, CoopMissionPtr> availableCoopMissions;
	
	map<unsigned int, MissionPtr> inProgressGoalMissions;
	map<unsigned int, CoopMissionPtr> inProgressCoopMissions;

	map<unsigned int, CoopMissionPtr> pendingCoopMissions;

	int IdGenerator;
	MissionManager() : IdGenerator{}, availableCoopMissions{}, availableGoalMissions{} {}
	MissionManager(MissionManager&) = delete;
	MissionManager& operator=(MissionManager&) = delete;
	~MissionManager() = default;

	

public:

	bool newGoalFound;
	static MissionManager& get() {
		return instance;
	}

	const map<unsigned int, MissionPtr>& getAvailableGoalMissions() {
		return availableGoalMissions;
	}
	const map<unsigned int, CoopMissionPtr>& getAvailableCoopMissions() {
		return availableCoopMissions;
	}

	void takeCoopMission(int missionId, int rId) {
		inProgressCoopMissions[missionId] = availableCoopMissions[missionId];
		inProgressCoopMissions[missionId]->receiverId = rId;
		availableCoopMissions.erase(missionId);
	}

	void takeGoalMission(int missionId) {
		inProgressGoalMissions[missionId] = availableGoalMissions[missionId];
		availableGoalMissions.erase(missionId);
	}

	void returnGoalMission(int missionId) {
#ifdef DEBUG
		if (std::find(availableGoals.begin(), availableGoals.end(), id) != availableGoals.end()) {
			throw new DuplicateObjectiveException;
		}
#endif // DEBUG
		if (missionId >= 0) {
			availableGoalMissions[missionId] = inProgressGoalMissions[missionId];
			inProgressGoalMissions.erase(missionId);
			newGoalFound = true;
		}
		
	}

	void returnCoopMission(int missionId) {
		availableCoopMissions[missionId] = inProgressCoopMissions[missionId];
		availableCoopMissions[missionId]->receiverId = -1;
		inProgressCoopMissions.erase(missionId);
	}

	void createGoalMission(int tileId) {
		int missionId = getNewId();
		availableGoalMissions[missionId] = make_shared<Mission>(missionId, tileId);
		newGoalFound = true;
	}

	void createCoopMission(int giverId, int ppId, int tileId) {
		int missionId = getNewId();
		availableCoopMissions[missionId] = make_shared<CoopMission>(missionId, giverId, ppId, tileId);
		newGoalFound = true;
	}

	void createPendingCoopMission(int giverId, int ppId) {
		int missionId = getNewId();
		availableCoopMissions[missionId] = make_shared<CoopMission>(missionId, giverId, ppId);
		newGoalFound = true;
	}

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

	template <class MPtr>
	int getBestMission(map<unsigned int, MPtr>&, Agent*);

	void updatePendingMissions();
};