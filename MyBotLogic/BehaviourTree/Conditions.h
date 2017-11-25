#pragma once
#include "Task.h"

class IsGoalReached : public Task {
	virtual Result execute(Agent *) override;
};

class IsPathValid : public Task {
	virtual Result execute(Agent *) override;
};

class IsNewPathNeeded : public Task {
	virtual Result execute(Agent *) override;
};