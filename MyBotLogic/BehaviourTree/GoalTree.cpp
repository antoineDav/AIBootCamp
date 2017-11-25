#include "GoalTree.h"

Task::Result GoalTree::execute(Agent * ag)
{
	return entry.execute(ag);
}

GoalTree::GoalTree()
{
	Selector* objSelector = new Selector();
	Discover* discover = new Discover();
	IsGoalReached* goalReached = new IsGoalReached();
	Sequence* nObjSequence = new Sequence();
	IsPathValid* pathValid = new IsPathValid();
	IsNewPathNeeded* newPathNeeded = new IsNewPathNeeded();
	FindObjective* findObj = new FindObjective();
	entry.addChild(objSelector);
	entry.addChild(discover);
	objSelector->addChild(goalReached);
	objSelector->addChild(nObjSequence);
	objSelector->addChild(pathValid);
	nObjSequence->addChild(newPathNeeded);
	nObjSequence->addChild(findObj);
}
