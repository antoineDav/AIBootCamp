#pragma once
#include "Task.h"
#include <memory>
#include <vector>

class Composite : public Task {
public:
	void addChild(Task*);

protected:
	std::vector<Task*> children;
};