#pragma once
#include "Task.h"

class DiscoverNear : public Task {
	virtual Result execute(Agent *) override;
};

class DiscoverFar : public Task {
	virtual Result execute(Agent *) override;
};
