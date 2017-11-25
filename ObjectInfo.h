#pragma once

#include "Globals.h"

#include <vector>

struct ObjectInfo
{
	unsigned int tileID;
	unsigned int objectID;
	Tile::ETilePosition position;
	std::set<Object::EObjectType> objectTypes;
	std::set<Object::EObjectState> objectStates;
	std::set<unsigned int> connectedTo;
};

