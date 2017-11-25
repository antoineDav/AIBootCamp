#pragma once

class Agent;
class Task {
public:
	enum Result {
		SUCCESS,
		FAILURE
	};
	virtual Result execute(Agent *) = 0;
};