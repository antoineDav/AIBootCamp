#include "Node.h"
#include <algorithm>
#include "GameManager.h"

Node::Node(unsigned int tileId, int colCount) noexcept
	: x{}
	, y{ (int)tileId / colCount  }
	, id{ (int)tileId }
	, type{ Tile::ETileType::TileAttribute_Unknown }
	, connectors{}
	, availableConnectors{}
{
	visited = false;
	x = 2*(static_cast<int>(tileId) % colCount) + y % 2;
}

void Node::addConnector(Connector* connector) noexcept {
	//Add target obj in connector
	if ((connector->getEndNodeC()->getType() != Tile::TileAttribute_Forbidden) && (getType() != Tile::TileAttribute_Forbidden)) {
		connectors.push_back(connector);
		availableConnectors.push_back(connector);
	}
}

void Node::popConnector(Node * obj) noexcept {
	//Pop target obj in connectors
	auto it = find_if(connectors.begin(), connectors.end(), [&](Connector* connector) {
		return (*connector->getEndNode() == *obj);
	});
	if (it != connectors.end()) {
		connectors.erase(it);
	}
}

void Node::popAvailableConnector(Node * obj) noexcept {
	//Pop target obj in connectors
	auto it = find_if(availableConnectors.begin(), availableConnectors.end(), [&](Connector* availableConnector) {
		return (*availableConnector->getEndNode() == *obj);
	});
	if (it != availableConnectors.end()) {
		availableConnectors.erase(it);
	}
}

Connector* Node::getConnector(Node * obj) noexcept {
	int i{};
	while (i < connectors.size()) {
		if (*connectors[i]->getEndNodeC() == *obj) {
			return connectors[i];
		}
		++i;
	}
	return nullptr;
}

Connector* Node::getConnector(Tile::ETilePosition dir) noexcept {
	//Pop target obj in connectors
	int i{};
	while (i < connectors.size()) {
		if (connectors[i]->getDirection() == dir) {
			return connectors[i];
		}
		++i;
	}
	return nullptr;
}

Connector* Node::getAvailableConnector(Node * obj) noexcept {
	int i{};
	while (i < availableConnectors.size()) {
		if (*availableConnectors[i]->getEndNodeC() == *obj) {
			return availableConnectors[i];
		}
		++i;
	}
	return nullptr;
}

Connector* Node::getAvailableConnector(Tile::ETilePosition dir) noexcept {
	//Pop target obj in connectors
	int i{};
	while (i < availableConnectors.size()) {
		if (availableConnectors[i]->getDirection() == dir) {
			return availableConnectors[i];
		}
		++i;
	}
	return nullptr;
}

