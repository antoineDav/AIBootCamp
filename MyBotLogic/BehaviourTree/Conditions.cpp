#include "Conditions.h"
#include "../Agent.h"
#include "../GameManager.h"
#include "../MissionManager.h"

Task::Result IsGoalReached::execute(Agent * ag)
{
	if (!ag->getIsSearching() && ag->getPos() == ag->getGoal()) {
		return this->SUCCESS;
	}
	else {
		return this->FAILURE;
	}
}

Task::Result IsPathValid::execute(Agent * ag)
{
	if (ag->getPathValid() && ag->getPath().size() >0) {
		return this->SUCCESS;
	}
	else {

		//If agent was on a mission, release it to be reassigned later
		if (ag->getMissionId() != -1) {

			MissionManager& mm = MissionManager::get();
			
			if (ag->getIsHelping()) {
				mm.returnMission(ag->getMissionId());
			}
			else {
				mm.returnMission(ag->getMissionId());
			}

			ag->setMissionId(-1, -1);
			ag->setHelping(false);

		}

		return this->FAILURE;
	}
}

Task::Result IsNewPathNeeded::execute(Agent * ag)
{
	if (!ag->getPathValid() || ag->getPath().size() == 0 /*|| MissionManager::get().isGoalFound()*/) {
		return this->SUCCESS;
	}
	else {
		return this->FAILURE;
	}
}
