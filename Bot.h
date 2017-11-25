#pragma once

#include "Globals.h"
#include "LevelInfo.h"
#include "TurnInfo.h"
#include "Logger.h"
#include <ctime>
#include <chrono>

#ifdef DEBUG_FRAMEWORK
	#define BOT_DEBUG
#endif

#ifdef BOT_DEBUG
	#define BOT_LOG(logger, text, autoEndLine) logger.Log(text, autoEndLine)
#else
	#define BOT_LOG(logger, text, autoEndLine) 0
#endif

class BotLogicIF;

class Bot
{

	enum EBotState
	{
		BotState_None,
		BotState_NotStarted,
		BotState_Starting,
		BotState_WaitingCommand,
		BotState_ProcessingLevelInfo,
		BotState_Initializing,
		BotState_ProcessingTurnInfo,
		BotState_PlayTurn,
		BotState_Exiting
	};

public:
	Bot(BotLogicIF* _pBotLogic);
	virtual ~Bot();

	void Configure(int argc, char *argv[]);
	void Run();
	

protected:
	BotLogicIF* mpBotLogic = NULL;
	EBotState mState = BotState_None;
	EBotState mNextState = BotState_None;
	bool mbCompleted = false;
	std::string mLogPath;

	LevelInfo mlevelInfo;
	TurnInfo* mpCurrentTurnInfo = NULL;

#ifdef BOT_DEBUG
	Logger mLogger;
#endif

#ifdef TURNINFO_DEBUG
	Logger mTurnInfoLogger;
#endif

	void SetBotLogic(BotLogicIF* _pBotLogic);

	void Update();
	
	void Start();
	void ProcessLevelInfo();
	void Init();
	void ProcessTurnInfo();
	void PlayTurn();
	void WaitCommand();
	void Exit();

	void SetNextState(EBotState _nextState);
	static const std::string& GetNextStateName(EBotState _botState);

	void Sleep(clock_t sec);
};

