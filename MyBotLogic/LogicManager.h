#pragma once

#include "State.h"
#include "State/MoveState.h"
#include "State/WaitState.h"
#include "BehaviourTree/GoalTree.h"

class LogicManager {
protected:
	LogicManager() = default;
private:
	static LogicManager instance;
	LogicManager(LogicManager&) = delete;
	LogicManager& operator=(LogicManager&) = delete;

	WaitState wait;
	MoveState move;

	GoalTree decisionTree;
public:
	static LogicManager& get() {
		return instance;
	}
	WaitState& getWaitState() {
		return wait;
	}
	MoveState& getMoveState() {
		return move;
	}
	GoalTree& getDecisionTree() {
		return decisionTree;
	}
};