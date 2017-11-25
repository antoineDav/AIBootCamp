#include "Sequence.h"
#include "../Agent.h"

Task::Result Sequence::execute(Agent * ag)
{
	for (const auto &child : children)
	{
		if (child->execute(ag) == Task::FAILURE) {
			return Task::FAILURE;
		}
	}
	return Task::SUCCESS;
}
