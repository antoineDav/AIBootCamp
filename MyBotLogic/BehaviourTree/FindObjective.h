#pragma once
#include "Task.h"

class FindObjective : public Task {
	virtual Result execute(Agent *) override;
};