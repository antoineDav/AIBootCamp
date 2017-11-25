#pragma once

#include <set>

struct NPCInfo
{
	unsigned int npcID = 0;
	float currentHP = 0.0f;
	float maxHP = 0.0f;
	unsigned int tileID = 0;
	bool omniscient = false;
	unsigned int visionRange = 0;
	unsigned int movementRange = 0;
	std::set<unsigned int> visibleTiles;
	std::set<unsigned int> visibleObjects;
};

