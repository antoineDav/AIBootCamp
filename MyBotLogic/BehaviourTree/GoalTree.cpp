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
	//Sequence* nObjSequence = new Sequence();
	IsPathValid* pathValid = new IsPathValid();
	IsNewPathNeeded* newPathNeeded = new IsNewPathNeeded();
	//FindObjective* findObj = new FindObjective();
	entry.addChild(objSelector);
	entry.addChild(discoverNear);
	entry.addChild(discoverDoor);
	entry.addChild(discoverFar);
	objSelector->addChild(goalReached);
	//objSelector->addChild(nObjSequence);
	objSelector->addChild(pathValid);
	//nObjSequence->addChild(newPathNeeded);
	//nObjSequence->addChild(findObj);
}
