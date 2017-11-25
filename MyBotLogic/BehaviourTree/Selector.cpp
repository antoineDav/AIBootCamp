#include "Selector.h"
#include "../Agent.h"

Task::Result Selector::execute(Agent * ag)
{
	for (const auto &child : children)
	{
		if (child->execute(ag) == Task::SUCCESS) {
			return Task::SUCCESS;
		}
	}
	return Task::FAILURE;
}
