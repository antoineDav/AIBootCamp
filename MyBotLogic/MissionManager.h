#include "MyBotLogic\Mission.h"
#include "MyBotLogic\Agent.h"

#include <vector>
#include <map>
#include <memory>

using namespace std;

class MissionManager {

public:

	using MissionPtr = shared_ptr<Mission>;

private:
	static MissionManager instance;

	map<unsigned int, MissionPtr> availableGoalMissions;
	map<unsigned int, MissionPtr> availableCoopMissions;
	
	map<unsigned int, MissionPtr> inProgressGoalMissions;
	map<unsigned int, MissionPtr> inProgressCoopMissions;

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
	const map<unsigned int, MissionPtr>& getAvailableCoopMissions() {
		return availableCoopMissions;
	}

	void takeCoopMission(int missionId) {
		inProgressCoopMissions[missionId] = availableCoopMissions[missionId];
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
		availableGoalMissions[missionId] = inProgressGoalMissions[missionId];
		inProgressGoalMissions.erase(missionId);
		newGoalFound = true;
	}

	void createGoalMission(int tileId) {
		int missionId = getNewId();
		availableGoalMissions[missionId] = make_shared<Mission>(missionId, tileId);
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

	int getBestMission(map<unsigned int, MissionPtr>&, Agent*);
};