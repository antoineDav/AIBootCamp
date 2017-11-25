#include "Node.h"
#include <algorithm>
#include "GameManager.h"

Node::Node(unsigned int tileId, int colCount) noexcept
	: x{}
	, y{ (int)tileId / colCount  }
	, id{ (int)tileId }
	, type{ Tile::ETileType::TileAttribute_Unknown }
	, connectors{}
{
	x = 2*(static_cast<int>(tileId) % colCount) + y % 2;
}

void Node::addConnector(Connector* connector) noexcept {
	//Add target obj in connector
	if ((connector->getEndNodeC()->getType() != Tile::TileAttribute_Forbidden) && (getType() != Tile::TileAttribute_Forbidden)) {
		connectors.push_back(connector);
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
