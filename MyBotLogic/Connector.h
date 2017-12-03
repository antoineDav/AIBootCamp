#pragma once
#include "Globals.h"
#include "Logger.h"

class Node;

class Connector {
private:
	Node * beginNode;
	Node * endNode;
	//Logger log;
	Tile::ETilePosition direction;
	int _objectId = -1;
	bool isToDestroy;
	bool isGrope = false;

public:
	void init(Node * begin, Node * end, Tile::ETilePosition direction);
	
	const Node * getBeginNodeC() const noexcept {
		return beginNode;
	}
	Node * getBeginNode() const noexcept {
		return beginNode;
	}
	const Node * getEndNodeC() const noexcept {
		return endNode;
	}
	Node * getEndNode() const noexcept {
		return endNode;
	}
	Tile::ETilePosition getDirection() const noexcept {
		return direction;
	}
	Tile::ETilePosition getInvertDirection() const noexcept {
		return (Tile::ETilePosition) ((direction + 3) % 6) ;
	}
	
	bool getIsToDestroy() const{
		return isToDestroy;
	}
	void setIsToDestroy(bool _isToDestroy) {
		isToDestroy = _isToDestroy;
	}

	int getObjects() const { return _objectId; }
	void setObject(int object) { _objectId= object; };
	bool hasObject() const noexcept { return _objectId != -1; }

	bool getIsGrope() { return isGrope; };
	void setIsGrope() { isGrope = true; };

	bool hasDoor() const noexcept;

};