#include "Discover.h"
#include "../Agent.h"
#include "../GameManager.h"

Task::Result Discover::execute(Agent * ag)
{
	ag->setPath(GameManager::get().getGraph().getBestUnkown(ag->getPos()));
	ag->setPathValid(true);
	ag->setGoal(ag->getPath()[0]->getEndNode()->getId());
	ag->setSearching(true);
	return SUCCESS;
}
