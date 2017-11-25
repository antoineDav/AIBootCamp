#pragma once

#include "TileInfo.h"
#include "Connector.h"
#include <vector>

class Node
{
private:
	//XY Coordinates
	int x;
	int y;

	int id;
	Tile::ETileType type;
	int _objectId;

	std::vector<Connector*> connectors; //Only connectors without any forbidden type on both end are stored


public:
	Node() = default;
	Node(unsigned int tileId, int colCount) noexcept;

	int getX() const noexcept {
		return x;
	}
	int getY() const noexcept {
		return y;
	}

	int getId() const noexcept {
		return id;
	}

	Tile::ETileType getType() const noexcept {
		return type;
	}
	void setType(Tile::ETileType _type) noexcept {
		type = _type;
	}

	std::vector<Connector*>* getConnectors() noexcept {
		return &connectors;
	}
	void clearConnectors() noexcept {
		connectors.clear();
	}
	bool isGoal() const noexcept {
		return getType() == Tile::TileAttribute_Goal;
	}

	int getObjects() { return _objectId; }
	void setObject(int object) { _objectId = object; };
	bool hasObject() { return _objectId != -1; }

	void addConnector(Connector* connector) noexcept;
	void popConnector(Node * obj) noexcept;
	Connector* getConnector(Node * obj) noexcept;
	Connector* getConnector(Tile::ETilePosition dir) noexcept;

	friend bool operator==(const Node& l, const Node& r) noexcept {
		return (l.getId() == r.getId());
	};
	friend bool operator!=(const Node& l, const Node& r) noexcept {
		return (l.getId() != r.getId());
	};
};