#include "Conditions.h"
#include "../Agent.h"
#include "../GameManager.h"

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
		return this->FAILURE;
	}
}

Task::Result IsNewPathNeeded::execute(Agent * ag)
{
	if (!ag->getPathValid() || ag->getPath().size() == 0 || GameManager::get().isGoalFound()) {
		return this->SUCCESS;
	}
	else {
		return this->FAILURE;
	}
}
