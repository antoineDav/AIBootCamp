#pragma once

#include <map>
#include <set>
#include <vector>

#include <iostream>
#include <string>

#include "Globals.h"

#include "NPCInfo.h"
#include "ObjectInfo.h"
#include "TileInfo.h"

#ifdef DEBUG_FRAMEWORK
   #define TURNINFO_DEBUG
#endif

class Logger;

struct TurnInfo
{
	unsigned int turnNb;
	std::map<unsigned int, ObjectInfo> objects;
	std::map<unsigned int, NPCInfo> npcs;
	std::map<unsigned int, TileInfo> tiles;

	std::string serializedData;

	void LogDebugInfo(Logger& _logger);
};

std::istream& operator>>(std::istream& _is, TurnInfo& _turnInfo);
