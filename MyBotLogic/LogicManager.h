#pragma once

#include "State.h"
#include "State/MoveState.h"
#include "State/WaitState.h"
#include "State/WaitCoopState.h"
#include "BehaviourTree/GoalTree.h"

class LogicManager {
protected:
	LogicManager() = default;
private:
	static LogicManager instance;
	LogicManager(LogicManager&) = delete;
	LogicManager& operator=(LogicManager&) = delete;

	WaitState wait;
	WaitCoopState waitCoop;
	MoveState move;

	GoalTree decisionTree;
public:
	static LogicManager& get() {
		return instance;
	}
	WaitState& getWaitState() {
		return wait;
	}
	WaitCoopState& getWaitCoopState() {
		return waitCoop;
	}

	MoveState& getMoveState() {
		return move;
	}
	GoalTree& getDecisionTree() {
		return decisionTree;
	}
};