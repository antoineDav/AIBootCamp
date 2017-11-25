cmake_minimum_required( VERSION 3.0 )
project( AIBot C CXX )

#add_definitions(-DDEBUG_FRAMEWORK)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
)

# Filters
include( Filters.cmake )


SET( AIBOT_BUILDFILES 

	ChangeLog.txt
	CheckInstall.bat
	CleanAll.bat
	CMakeLists.txt
	Filters.cmake
	GenAIBootCampViewerFiles.bat
	GenAIBootCampViewerFiles.py
	INSTALL.txt
	PlayLocalMatch.bat
	rebuild_vis_sln_x64.bat
	SubmitAIBot.bat
	SubmitAIBot.py
	TeamInfo.team
)

SET( AIBOT_HEADERS 

	#Framework
	Bot.h	
	BotLogicIF.h
	Globals.h
	LevelInfo.h
	Logger.h
	NPCInfo.h
	ObjectInfo.h
	TileInfo.h
	TurnInfo.h

	#MY_BOT
	MyBotLogic/MyBotLogic.h
	MyBotLogic/Graph.h
	MyBotLogic/Node.h
	MyBotLogic/Agent.h
	MyBotLogic/Connector.h
	MyBotLogic/State.h
	MyBotLogic/GameManager.h
	MyBotLogic/Utility.h
	MyBotLogic/LogicManager.h
	
	MyBotLogic/State/WaitState.h
	MyBotLogic/State/MoveState.h
	
	MyBotLogic/BehaviourTree/Composite.h
	MyBotLogic/BehaviourTree/Conditions.h
	MyBotLogic/BehaviourTree/Discover.h
	MyBotLogic/BehaviourTree/FindObjective.h
	MyBotLogic/BehaviourTree/GoalTree.h
	MyBotLogic/BehaviourTree/Selector.h
	MyBotLogic/BehaviourTree/Sequence.h
	MyBotLogic/BehaviourTree/Task.h
)

SET( AIBOT_SRC 

	#Framework
	Bot.cpp
	LevelInfo.cpp
	Logger.cpp
	TurnInfo.cpp

	#MY_BOT
	MyBotLogic/MyBotLogic.cpp
	MyBotLogic/Graph.cpp
	MyBotLogic/Node.cpp
	MyBotLogic/Agent.cpp
	MyBotLogic/Connector.cpp
	MyBotLogic/State.cpp
	MyBotLogic/GameManager.cpp
	MyBotLogic/LogicManager.cpp
	
	MyBotLogic/State/WaitState.cpp
	MyBotLogic/State/MoveState.cpp
	
	MyBotLogic/BehaviourTree/Composite.cpp
	MyBotLogic/BehaviourTree/Conditions.cpp
	MyBotLogic/BehaviourTree/Discover.cpp
	MyBotLogic/BehaviourTree/FindObjective.cpp
	MyBotLogic/BehaviourTree/GoalTree.cpp
	MyBotLogic/BehaviourTree/Selector.cpp
	MyBotLogic/BehaviourTree/Sequence.cpp
	MyBotLogic/BehaviourTree/Task.cpp
	
	#Main
	Main.cpp
)

SET(jobfiles "${AIBOT_HEADERS};${AIBOT_SRC};${AIBOT_BUILDFILES}")
file(WRITE jobfiles.txt "${jobfiles}")

add_executable(
	aibot

	${AIBOT_BUILDFILES}
	${AIBOT_HEADERS}
	${AIBOT_SRC}
)

CREATE_VISUAL_STUDIO_FILTERS(AIBOT)

