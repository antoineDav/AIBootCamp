#include "GoalTree.h"

Task::Result GoalTree::execute(Agent * ag)
{
	return entry.execute(ag);
}

GoalTree::GoalTree()
{
	Selector* objSelector = new Selector(); 
	DiscoverNear* discoverNear = new DiscoverNear();
	DiscoverFar* discoverFar = new DiscoverFar();
	DiscoverDoor* discoverDoor = new DiscoverDoor();
	IsGoalReached* goalReached = new IsGoalReached();
	IsPathValid* pathValid = new IsPathValid();
	IsNewPathNeeded* newPathNeeded = new IsNewPathNeeded();
	entry.addChild(objSelector);
	entry.addChild(discoverNear);
	entry.addChild(discoverFar);
	entry.addChild(discoverDoor);
	objSelector->addChild(goalReached);
	objSelector->addChild(pathValid);
}
