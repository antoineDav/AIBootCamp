#include "Composite.h"
#include "../Agent.h"

void Composite::addChild(Task* child)
{
	children.push_back(child);
}
