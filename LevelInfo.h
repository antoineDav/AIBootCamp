#pragma once

#include <iostream>
#include <map>
#include <string>

#include "NPCInfo.h"
#include "ObjectInfo.h"
#include "TileInfo.h"

#ifdef DEBUG_FRAMEWORK
	#define LEVELINFO_DEBUG
#endif

class Logger;

struct LevelInfo
{	
	int rowCount;
	int colCount;
	int loadDelay;
	int initDelay;
	int turnDelay;
	int maxTurnNb;
	std::map<unsigned int, ObjectInfo> objects;
	std::map<unsigned int, NPCInfo> npcs;
	std::map<unsigned int, TileInfo> tiles;

	std::string serializedData;

	void LogDebugInfo(Logger& _logger);

};

std::istream& operator>>(std::istream& _is, LevelInfo& _levelInfo);

