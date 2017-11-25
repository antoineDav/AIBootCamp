#include "Bot.h"

#include <assert.h>
#include <vector>

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <chrono>

#include <windows.h>

#include "BotLogicIF.h"
#include "LevelInfo.h"
#include "TurnInfo.h"

#define FSM_MAX_NB_ITERATION 50

Bot::Bot(BotLogicIF* _pBotLogic)
	: mpBotLogic(_pBotLogic)
	, mLogPath("./aibot")
{

}

Bot::~Bot()
{
	BOT_LOG(mLogger, "~Bot\n", true);

	if (mpBotLogic != nullptr)
	{
		delete mpBotLogic;
	}
}

void Bot::SetBotLogic(BotLogicIF* _pBotLogic)
{
	BOT_LOG(mLogger, "SetBotLogic", true);

	if (mpBotLogic != nullptr)
	{
		delete mpBotLogic;
	}
	mpBotLogic = _pBotLogic;
}

void Bot::Run()
{
	while (!mbCompleted)
	{
		Update();
	}
}

void Bot::Update()
{
	do 
	{
		switch (mState)
		{
			case BotState_NotStarted:
			{
				SetNextState(BotState_Starting);
			}
			break;
			
			case BotState_Starting:
			{
				Start();
			}
			break;

			case BotState_WaitingCommand:
			{
				WaitCommand();
			}
			break;

			case BotState_ProcessingLevelInfo:
			{
				ProcessLevelInfo();
			}
			break;

			case BotState_Initializing:
			{
				Init();
			}
			break;

			case BotState_ProcessingTurnInfo:
			{
				ProcessTurnInfo();
			}
			break;

			case BotState_PlayTurn:
			{
				PlayTurn();
			}
			break;

			case BotState_Exiting:
			{
				Exit();
			}
			break;

			default:
				break;
		}

		if (mState != mNextState)
		{
			mState = mNextState;
		}
		else
		{
			break;
		}
	} while (true);	
}

void Bot::Configure(int argc, char *argv[])
{
	if (argc >= 3)
	{
		std::string switchStr(argv[1]);
		if (switchStr == "-AIBotLogPath")
		{
			std::string path(argv[2]);
			mLogPath = path;
		}
	}

	CreateDirectory(mLogPath.c_str(), NULL);

#ifdef BOT_DEBUG
	mLogger.Init(mLogPath, "BotDebug.txt");
#endif

#ifdef TURNINFO_DEBUG
	mTurnInfoLogger.Init(mLogPath, "TurnInfoDebug.txt");
#endif
	
	BOT_LOG(mLogger, "Configure", true);

	assert(mpBotLogic != nullptr && "mpBotLogic cannot be null at this point. Forgot to assign a botlogic in Main.cpp? ");
	
	mpBotLogic->Configure(argc, argv, mLogPath);

	SetNextState(BotState_NotStarted);
}

void Bot::Start()
{
	BOT_LOG(mLogger, "Start", true);

	assert(mpBotLogic != nullptr && "mpBotLogic cannot be null at this point. Forgot to assign a botlogic in Main.cpp? ");

	mpBotLogic->Start();

	std::cout << "AIBotStarted" << std::endl;

	SetNextState(BotState_WaitingCommand);
}

void Bot::ProcessLevelInfo()
{
	BOT_LOG(mLogger, "ProcessLevelInfo", true);
	
	assert(mpBotLogic != nullptr && "mpBotLogic cannot be null at this point. Forgot to assign a botlogic in Main.cpp? ");
	
	std::cin >> mlevelInfo;

#ifdef LEVELINFO_DEBUG
	Logger logger;
	logger.Init(mLogPath, "LevelInfoDebug.txt");
	mlevelInfo.LogDebugInfo(logger);
#endif
	

#ifdef BOT_DEBUG
	std::stringstream ss;
	ss << "LevelInfo" << std::endl;

	ss << "\tMap Size : (" << mlevelInfo.rowCount << ", " << mlevelInfo.colCount << ")" << std::endl;
	ss << "\tLoadDelay : "<< mlevelInfo.loadDelay << std::endl;
	ss << "\tInitDelay : " << mlevelInfo.initDelay << std::endl;
	ss << "\tTurnDelay : " << mlevelInfo.turnDelay << std::endl;
	ss << "\tMaxNbTurn : " << mlevelInfo.maxTurnNb << std::endl;

	ss << "\tNPCs (" << mlevelInfo.npcs.size() << ") : [" << std::endl;

	for (std::map<unsigned int, NPCInfo>::iterator it = mlevelInfo.npcs.begin(); it != mlevelInfo.npcs.end(); ++it)
	{
		NPCInfo& npcInfo = it->second;
		ss << "\t   NPC UID : " << npcInfo.npcID << std::endl;
		ss << "\t      Omniscient : " << npcInfo.omniscient << std::endl;
		ss << "\t      Vision Range : " << npcInfo.visionRange << std::endl;
		ss << "\t      Movement Range : " << npcInfo.movementRange << std::endl;
		ss << "\t      Current HP : " << npcInfo.currentHP << std::endl;
		ss << "\t      Max HP : " << npcInfo.maxHP << std::endl;
		ss << "\t      TileID : " << npcInfo.tileID << std::endl;

		ss << "\t      Visible Objects (" << npcInfo.visibleObjects.size() << ") : [";
		for (std::set<unsigned int>::iterator it = npcInfo.visibleObjects.begin(); it != npcInfo.visibleObjects.end(); ++it)
		{
			ss << *it << " ";
		}
		ss << "]" << std::endl;

		ss << "\t      Visible Tiles (" << npcInfo.visibleTiles.size() << ") : [";
		for (std::set<unsigned int>::iterator it = npcInfo.visibleTiles.begin(); it != npcInfo.visibleTiles.end(); ++it)
		{
			ss << *it << " ";
		}
		ss << "]" << std::endl;
	}
	
	ss << "\t]" << std::endl;

	ss << "\tObjects (" << mlevelInfo.objects.size() << ") : [" << std::endl;

	for (std::map<unsigned int, ObjectInfo>::iterator it = mlevelInfo.objects.begin(); it != mlevelInfo.objects.end(); ++it)
	{
		ObjectInfo& objectInfo = it->second;
		ss << "\t   Object UID : " << objectInfo.objectID << std::endl;
		ss << "\t      TileID : " << objectInfo.tileID << std::endl;
		ss << "\t      PositionID : ";
		switch (objectInfo.position)
		{
		case Tile::ETilePosition::NE:
			ss << "NE";
			break;
		case Tile::ETilePosition::E:
			ss << "E";
			break;
		case Tile::ETilePosition::SE:
			ss << "SE";
			break;
		case Tile::ETilePosition::SW:
			ss << "SW";
			break;
		case Tile::ETilePosition::W:
			ss << "W";
			break;
		case Tile::ETilePosition::NW:
			ss << "NW";
			break;
		case Tile::ETilePosition::CENTER:
			ss << "CENTER";
			break;
		default:
			break;
		}	
		ss << std::endl;
		
		ss << "\t      Tags : [";
		for (std::set<Object::EObjectType>::iterator objTypeIt = objectInfo.objectTypes.begin(); objTypeIt != objectInfo.objectTypes.end(); ++objTypeIt)
		{
			Object::EObjectType objType = *objTypeIt;
			switch (objType)
			{
			case Object::ObjectType_Door:
				ss << "Door ";
				break;
			case Object::ObjectType_Wall:
				ss << "Wall ";
				break;
			case Object::ObjectType_PressurePlate:
				ss << "PressurePlate ";
				break;
			case Object::ObjectType_Window:
				ss << "Window ";
				break;
			default:
				ss << "Unknown ";
				break;
			}
		}
		ss << "]" << std::endl;
		
		ss << "\t      States : [";
		for (std::set<Object::EObjectState>::iterator objStateIt = objectInfo.objectStates.begin(); objStateIt != objectInfo.objectStates.end(); ++objStateIt)
		{
			Object::EObjectState objState = *objStateIt;
			switch (objState)
			{
			case Object::ObjectState_Closed:
				ss << "Closed ";
				break;
			case Object::ObjectState_Opened:
				ss << "Opened ";
				break;
			default:
				ss << "Undefined ";
				break;
			}
		}
		ss << "]" << std::endl;

		ss << "\t      ConnectedTo : [";
		for (std::set<unsigned int>::iterator connectedToIt = objectInfo.connectedTo.begin(); connectedToIt != objectInfo.connectedTo.end(); ++connectedToIt)
		{
			ss << *connectedToIt << " ";
		}
		ss << "]" << std::endl;
	}

	ss << "\t]" << std::endl;

	ss << "\tTiles (" << mlevelInfo.tiles.size() << ") : [" << std::endl;

	for (std::map<unsigned int, TileInfo>::iterator it = mlevelInfo.tiles.begin(); it != mlevelInfo.tiles.end(); ++it)
	{
		TileInfo& tileInfo = it->second;
		ss << "\t   Tile UID : " << tileInfo.tileID << ", ";
		
		switch (tileInfo.tileType)
		{
		default:
			break;
		case Tile::TileAttribute_Default:
			ss << "Default";
			break;
		case Tile::TileAttribute_Forbidden:
			ss << "Forbidden";
			break;
		case Tile::TileAttribute_Omniscient:
			ss << "Omniscient";
			break;
		case Tile::TileAttribute_Goal:
			ss << "Goal";
			break;
		}
		
		ss << std::endl;
	}

	ss << "\t]" << std::endl;

	BOT_LOG(mLogger, ss.str(), true);
#endif

	std::cout << "AIBotLevelInfoProcessed" << std::endl;

	SetNextState(BotState_WaitingCommand);
}

void Bot::Init()
{
	BOT_LOG(mLogger, "Init", true);

	assert(mpBotLogic != nullptr && "mpBotLogic cannot be null at this point. Forgot to assign a botlogic in Main.cpp? ");

	BOT_LOG(mLogger, "Init BotLogic", true);

	mpBotLogic->Init(mlevelInfo);

	std::cout << "AIBotInitialized" << std::endl;

	SetNextState(BotState_WaitingCommand);

}

void Bot::ProcessTurnInfo()
{
	BOT_LOG(mLogger, "ProcessTurnInfo", true);

	assert(mpBotLogic != nullptr && "mpBotLogic cannot be null at this point. Forgot to assign a botlogic in Main.cpp? ");

	if (mpCurrentTurnInfo != NULL)
	{
		delete mpCurrentTurnInfo;
		mpCurrentTurnInfo = NULL;
	}

	mpCurrentTurnInfo = new TurnInfo();

	std::cin >> *mpCurrentTurnInfo;

#ifdef TURNINFO_DEBUG
	mpCurrentTurnInfo->LogDebugInfo(mTurnInfoLogger);
	mTurnInfoLogger.Log("", true);
#endif

#ifdef BOT_DEBUG
	std::stringstream ss;
	ss << "TurnInfo" << std::endl;

	ss << "\tTurnNb : " << mpCurrentTurnInfo->turnNb << std::endl;

	ss << "\tNPCs (" << mpCurrentTurnInfo->npcs.size() << ") : [" << std::endl;

	for (std::map<unsigned int, NPCInfo>::iterator it = mpCurrentTurnInfo->npcs.begin(); it != mpCurrentTurnInfo->npcs.end(); ++it)
	{
		NPCInfo& npcInfo = it->second;
		ss << "\t   NPC UID : " << npcInfo.npcID << std::endl;
		ss << "\t      Omniscient : " << npcInfo.omniscient << std::endl;
		ss << "\t      Vision Range : " << npcInfo.visionRange << std::endl;
		ss << "\t      Movement Range : " << npcInfo.movementRange << std::endl;
		ss << "\t      Current HP : " << npcInfo.currentHP << std::endl;
		ss << "\t      Max HP : " << npcInfo.maxHP << std::endl;
		ss << "\t      TileID : " << npcInfo.tileID << std::endl;

		ss << "\t      Visible Objects (" << npcInfo.visibleObjects.size() << ") : [";
		for (std::set<unsigned int>::iterator it = npcInfo.visibleObjects.begin(); it != npcInfo.visibleObjects.end(); ++it)
		{
			ss << *it << " ";
		}
		ss << "]" << std::endl;

		ss << "\t      Visible Tiles (" << npcInfo.visibleTiles.size() << ") : [";
		for (std::set<unsigned int>::iterator it = npcInfo.visibleTiles.begin(); it != npcInfo.visibleTiles.end(); ++it)
		{
			ss << *it << " ";
		}
		ss << "]" << std::endl;
	}

	ss << "\t]" << std::endl;

	ss << "\tObjects (" << mpCurrentTurnInfo->objects.size() << ") : [" << std::endl;

	for (std::map<unsigned int, ObjectInfo>::iterator it = mpCurrentTurnInfo->objects.begin(); it != mpCurrentTurnInfo->objects.end(); ++it)
	{
		ObjectInfo& objectInfo = it->second;
		ss << "\t   Object UID : " << objectInfo.objectID << std::endl;
		ss << "\t      TileID : " << objectInfo.tileID << std::endl;
		ss << "\t      PositionID : ";
		switch (objectInfo.position)
		{
		case Tile::ETilePosition::NE:
			ss << "NE";
			break;
		case Tile::ETilePosition::E:
			ss << "E";
			break;
		case Tile::ETilePosition::SE:
			ss << "SE";
			break;
		case Tile::ETilePosition::SW:
			ss << "SW";
			break;
		case Tile::ETilePosition::W:
			ss << "W";
			break;
		case Tile::ETilePosition::NW:
			ss << "NW";
			break;
		case Tile::ETilePosition::CENTER:
			ss << "CENTER";
			break;
		default:
			break;
		}
		ss << std::endl;

		ss << "\t      Tags : [";
		for (std::set<Object::EObjectType>::iterator objTypeIt = objectInfo.objectTypes.begin(); objTypeIt != objectInfo.objectTypes.end(); ++objTypeIt)
		{
			Object::EObjectType objType = *objTypeIt;
			switch (objType)
			{
			case Object::ObjectType_Door:
				ss << "Door ";
				break;
			case Object::ObjectType_Wall:
				ss << "Wall ";
				break;
			case Object::ObjectType_PressurePlate:
				ss << "PressurePlate ";
				break;
			case Object::ObjectType_Window:
				ss << "Window ";
				break;
			default:
				ss << "Unknown ";
				break;
			}
		}
		ss << "]" << std::endl;

		ss << "\t      States : [";
		for (std::set<Object::EObjectState>::iterator objStateIt = objectInfo.objectStates.begin(); objStateIt != objectInfo.objectStates.end(); ++objStateIt)
		{
			Object::EObjectState objState = *objStateIt;
			switch (objState)
			{
			case Object::ObjectState_Closed:
				ss << "Closed ";
				break;
			case Object::ObjectState_Opened:
				ss << "Opened ";
				break;
			default:
				ss << "Undefined ";
				break;
			}
		}
		ss << "]" << std::endl;

		ss << "\t      ConnectedTo : [";
		for (std::set<unsigned int>::iterator connectedToIt = objectInfo.connectedTo.begin(); connectedToIt != objectInfo.connectedTo.end(); ++connectedToIt)
		{
			ss << *connectedToIt << " ";
		}
		ss << "]" << std::endl;
	}

	ss << "\t]" << std::endl;

	ss << "\tTiles (" << mpCurrentTurnInfo->tiles.size() << ") : [" << std::endl;

	for (std::map<unsigned int, TileInfo>::iterator it = mpCurrentTurnInfo->tiles.begin(); it != mpCurrentTurnInfo->tiles.end(); ++it)
	{
		TileInfo& tileInfo = it->second;
		ss << "\t   Tile UID : " << tileInfo.tileID << ", ";

		switch (tileInfo.tileType)
		{
		default:
			break;
		case Tile::TileAttribute_Default:
			ss << "Default";
			break;
		case Tile::TileAttribute_Forbidden:
			ss << "Forbidden";
			break;
		case Tile::TileAttribute_Omniscient:
			ss << "Omniscient";
			break;
		case Tile::TileAttribute_Goal:
			ss << "Goal";
			break;
		}

		ss << std::endl;
	}

	ss << "\t]" << std::endl;

	BOT_LOG(mLogger, ss.str(), true);
#endif

	std::cout << "AIBotTurnInfoProcessed" << std::endl;

	SetNextState(BotState_WaitingCommand);
}

void Bot::PlayTurn()
{
	BOT_LOG(mLogger, "PlayTurn", true);

	assert(mpBotLogic != nullptr && "mpBotLogic cannot be null at this point. Forgot to assign a botlogic in Main.cpp? ");
	assert(mpCurrentTurnInfo != NULL);

	std::vector<Action*> actions;
		
	mpBotLogic->FillActionList(*mpCurrentTurnInfo, actions);

	std::stringstream actionSS;
	actionSS << "TurnActionsInfo " << actions.size();

	for each (auto* pAction in actions)
	{
		if (pAction->actionType == Action::ActionType_Move)
		{
			Move* pMove = (Move*)pAction;
			actionSS << " " << pMove->unitID << " " << (int)pMove->actionType << " " << (int)pMove->direction;
		}

		if (pAction->actionType == Action::ActionType_Interact)
		{
			Interact* pInteract = (Interact*)pAction;
			actionSS << " " << pInteract->unitID << " " << (int)pInteract->actionType << " " << (int)pInteract->objectID << " " << (int)pInteract->interaction;
		}

		delete pAction;
	}

	std::cout << actionSS.str() << std::endl;

#ifdef BOT_DEBUG
	std::stringstream ss2;
	ss2 << "\t Actions : " << " " << actionSS.str();
	BOT_LOG(mLogger, ss2.str(), true);
#endif

	SetNextState(BotState_WaitingCommand);
}

void Bot::Sleep(clock_t sec)
{
	clock_t start_time = clock();
	clock_t end_time = sec * 1000 + start_time;
	while (clock() != end_time);
}

void Bot::WaitCommand()
{
	BOT_LOG(mLogger, "WaitCommand...", false);

	char c = std::cin.peek();
	if (c == EOF)
	{
		return;
	}

	std::string cmd;
	std::getline(std::cin, cmd);

	BOT_LOG(mLogger, cmd, true);

	if (cmd == "[ProcessLevelInfo]")
	{
		SetNextState(BotState_ProcessingLevelInfo);
	}
	else if (cmd == "[Init]")
	{
		SetNextState(BotState_Initializing);
	}
	else if (cmd == "[ProcessTurnInfo]")
	{
		SetNextState(BotState_ProcessingTurnInfo);
	}
	else if (cmd == "[PlayTurn]")
	{
		SetNextState(BotState_PlayTurn);
	}
	else if (cmd == "[Exit]")
	{
		SetNextState(BotState_Exiting);
	}
	else
	{
		assert(false);
	}
}

void Bot::Exit()
{
	BOT_LOG(mLogger, "Exit", false);

	assert(mpBotLogic != nullptr && "mpBotLogic cannot be null at this point. Forgot to assign a botlogic in Main.cpp? ");

	//ExitInfo exitInfo;
	//std::cin >> exitInfo;

	mpBotLogic->Exit();

	BOT_LOG(mLogger, "BotLogic Exit", false);

	std::cout << "ExitProcessed" << std::endl;

	mbCompleted = true;
}

void Bot::SetNextState(EBotState _nextState)
{
	mNextState = _nextState;

#ifdef BOT_DEBUG
	std::stringstream ss;
	ss << "SetNextState(" << GetNextStateName(mNextState) << ")" << std::endl;

	BOT_LOG(mLogger, ss.str(), false);
#endif
}

const std::string& Bot::GetNextStateName(EBotState _botState)
{
	static std::string sBotState_None			= "None";
	static std::string sBotState_NotStarted		= "NotStarted";
	static std::string sBotState_Starting		= "Starting";
	static std::string sBotState_WaitingCommand = "WaitingCommand";
	static std::string sBotState_Initializing	= "Initializing";
	static std::string sBotState_PlayTurn		= "PlayTurn";
	static std::string sBotState_Exiting		= "Exiting";
	static std::string sBotState_ProcessingLevelInfo = "ProcessLevelInfo";
	static std::string sBotState_ProcessingTurnInfo = "ProcessTurnInfo";
	
	static std::string sError					= "INVALID";

	switch (_botState)
	{
		case BotState_None:
		{
			return sBotState_None;
		}
		break;
		
		case BotState_NotStarted:
		{
			return sBotState_NotStarted;
		}
		break;

		case BotState_Starting:
		{
			return sBotState_Starting;
		}
		break;

		case BotState_WaitingCommand:
		{
			return sBotState_WaitingCommand;
		}
		break;

		case BotState_ProcessingLevelInfo:
		{
			return sBotState_ProcessingLevelInfo;
		}
		break;

		case BotState_Initializing:
		{
			return sBotState_Initializing;
		}
		break;

		case BotState_ProcessingTurnInfo:
		{
			return sBotState_ProcessingTurnInfo;
		}
		break;

		case BotState_PlayTurn:
		{
			return sBotState_PlayTurn;
		}
		break;

		case BotState_Exiting:
		{
			return sBotState_Exiting;
		}
		break;

		default:
			break;
	}
	return sError;
}
