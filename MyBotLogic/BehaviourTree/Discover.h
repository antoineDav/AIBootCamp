#pragma once
#include "Task.h"

class Discover : public Task {
	virtual Result execute(Agent *) override;
};