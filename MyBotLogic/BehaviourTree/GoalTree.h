#pragma once
#include "Composite.h"
#include "Conditions.h"
#include "Discover.h"
#include "FindObjective.h"
#include "Selector.h"
#include "Sequence.h"
#include "Task.h"

class GoalTree {
public:
	Task::Result execute(Agent*);
	GoalTree();
private:
	static GoalTree instance;
	Selector entry;
};