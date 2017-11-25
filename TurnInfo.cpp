#include "TurnInfo.h"

#include <string>
#include <sstream>
#include <fstream>

#include <chrono>
#include <thread>

#include "Logger.h"

void TurnInfo::LogDebugInfo(Logger& _logger)
{
	_logger.Log(serializedData, true);
}

std::istream& operator>>(std::istream& _is, TurnInfo& _turnInfo)
{
	std::getline(_is, _turnInfo.serializedData);

	std::fstream log;
	log.open("D:/turntime.txt", std::ofstream::out);
	
	
	auto start = std::chrono::high_resolution_clock::now();

	std::stringstream ss(_turnInfo.serializedData);

	std::string varName;
	while (!ss.eof())
	{
		ss >> varName;
		
		if (varName == "TurnNb")
		{
			ss >> _turnInfo.turnNb;
		}
		else if (varName == "NPCCount")
		{
			int nbNPCs = 0;
			ss >> nbNPCs;

			for (int i = 0; i < nbNPCs; ++i)
			{
				NPCInfo npcInfo;

				ss >> npcInfo.npcID;
				ss >> npcInfo.omniscient;
				ss >> npcInfo.visionRange;
				ss >> npcInfo.movementRange;
				ss >> npcInfo.currentHP;
				ss >> npcInfo.maxHP;
				ss >> npcInfo.tileID;

				int nbTileCnt = 0;
				int nbObjectCnt = 0;

				int val = 0;

				ss >> varName;
				if (varName == "VisibleTileCount")
				{
					ss >> nbTileCnt;
					for (int j = 0; j < nbTileCnt; ++j)
					{
						unsigned int tileID = 0;
						ss >> tileID;
						npcInfo.visibleTiles.insert(tileID);
					}
				}

				ss >> varName;
				if (varName == "VisibleObjectCount")
				{
					ss >> nbObjectCnt;
					for (int j = 0; j < nbObjectCnt; ++j)
					{
						unsigned int objectID = 0;
						ss >> objectID;
						npcInfo.visibleObjects.insert(objectID);
					}
				}

				_turnInfo.npcs[npcInfo.npcID] = npcInfo;
			}
			varName = "";
		}

		if (varName == "VisibleObjectCount")
		{
			int nbObjects = 0;
			ss >> nbObjects;

			std::string objectType;
			std::string objectState;

			for (int i = 0; i < nbObjects; ++i)
			{
				ObjectInfo objectInfo;

				ss >> objectInfo.objectID;
				ss >> objectInfo.tileID;

				int position;
				ss >> position;
				objectInfo.position = (Tile::ETilePosition)position;

				//for (int j = 0; j < 8; ++j)
				//{
				//	ss >> objectInfo.edgesCost[j];
				//}

				int nbTags;
				ss >> nbTags;

				for (int j = 0; j < nbTags; ++j)
				{
					ss >> objectType;
					if (objectType == "wall")
					{
						objectInfo.objectTypes.insert(Object::EObjectType::ObjectType_Wall);
					}
					else if (objectType == "window")
					{
						objectInfo.objectTypes.insert(Object::EObjectType::ObjectType_Window);
					}
					else if (objectType == "door")
					{
						objectInfo.objectTypes.insert(Object::EObjectType::ObjectType_Door);
					}
					else if (objectType == "pressureplate")
					{
						objectInfo.objectTypes.insert(Object::EObjectType::ObjectType_PressurePlate);
					}
				}

				int nbStates;
				ss >> nbStates;

				for (int j = 0; j < nbStates; ++j)
				{
					ss >> objectState;
					if (objectState == "closed")
					{
						objectInfo.objectStates.insert(Object::EObjectState::ObjectState_Closed);
					}
					else if (objectState == "opened")
					{
						objectInfo.objectStates.insert(Object::EObjectState::ObjectState_Opened);
					}
				}

				int nbConnectedObj = 0;
				ss >> nbConnectedObj;

				for (int j = 0; j < nbConnectedObj; ++j)
				{
					unsigned int controllerUID = 0;
					ss >> controllerUID;
					objectInfo.connectedTo.insert(controllerUID);
				}

				_turnInfo.objects[objectInfo.objectID] = objectInfo;
			}
		}

		if (varName == "VisibleTileCount")
		{
			int nbTileCnt = 0;

			TileInfo tileInfo;

			int val = 0;

			ss >> nbTileCnt;
			for (int j = 0; j < nbTileCnt; ++j)
			{
				ss >> tileInfo.tileID;

				ss >> val;
				tileInfo.tileType = (Tile::ETileType)(val);

				_turnInfo.tiles[tileInfo.tileID] = tileInfo;
			}
		}
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

	log << duration << std::endl;
	log.flush();

	return _is;
}

