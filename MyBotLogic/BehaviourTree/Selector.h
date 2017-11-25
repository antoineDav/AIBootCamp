#pragma once
#include "Composite.h"

class Selector : public Composite {
public:
	virtual Task::Result execute(Agent *) override;
};