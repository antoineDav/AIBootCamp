#pragma once
#include "Composite.h"

class Sequence : public Composite {
public:
	virtual Task::Result execute(Agent *) override;
};