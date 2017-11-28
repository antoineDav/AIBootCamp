#include "Discover.h"
#include "../Agent.h"
#include "../GameManager.h"

Task::Result DiscoverNear::execute(Agent * ag)
{
	ag->setPath(GameManager::get().getGraph().getNearUnkown(ag->getPos()));
	if (ag->getPath().size() > 0)
	{
		ag->setPathValid(true);
		ag->setGoal(ag->getPath()[0]->getEndNode()->getId());
		ag->setSearching(true);
		return SUCCESS;
	}
	else
	{
		return FAILURE;
	}	
}

Task::Result DiscoverFar::execute(Agent * ag)
{
	ag->setPath(GameManager::get().getGraph().getFarUnkown(ag->getPos()));
	ag->setPathValid(true);
	ag->setGoal(ag->getPath()[0]->getEndNode()->getId());
	ag->setSearching(true);
	return SUCCESS;	
}
