#pragma once

#include "Node.h"
#include <map>
#include "TurnInfo.h"
#include "ObjectInfo.h"

using namespace std;

class Graph
{
private:
	int rowCount;
	int colCount;
	vector<Node> nodes;
	vector<Connector> connectors;
	vector<Connector*> forbiddenConnector;
	vector<Connector*> wallConnector;
	map<unsigned int, ObjectInfo> objects;

private:
	Node& getNode(int id) noexcept {
		return nodes[id];
	}
	int getPositionId(int x, int y) const noexcept; 

	//Try to add to the node the connector based on the dir and the x/y of the obj node
	void tryAddConnector(Node& node, Tile::ETilePosition dir, int x, int y, int& i) noexcept;

	//Create the nodes without their connectors with the Node constructor
	void createNodes() noexcept;
	//Create all connectors empty
	void createConnectors() noexcept;

	//Init the connectors for each accessible neighbours for each node
	void initConnectors() noexcept;
	//Update nodes
	void updateNodesType(const std::map<unsigned int, TileInfo>& tiles) noexcept;
	void updateNodeObjects(const std::map<unsigned int, ObjectInfo>& in_objects);
	//Update connectors and delete unaccessible connectors
	void updateConnectorsWithType(const std::map<unsigned int, TileInfo>& tiles) noexcept;
	void updateConnectorsWithObjects(const std::map<unsigned int, ObjectInfo>& objects) noexcept;

public:
	void init(int _rowCount, int _colCount, const std::map<unsigned int, TileInfo>& tiles, const std::map<unsigned int, ObjectInfo>& objects) noexcept;

	//Mets à jour les infos des tiles et invalide les connectors à invalider
	void update(const map<unsigned int, TileInfo>& tiles, const std::map<unsigned int, ObjectInfo>& objects) noexcept;
	
private:
	class HeuristicManhattan;
	struct NodeItem;
	class NodeItemPtrComparison;

public:
	vector<int> getGoalPosition() const noexcept;
	int dist(int n1, int n2) const noexcept {
		const Node* node1{ &nodes[n1] };
		const Node* node2{ &nodes[n2] };
		return (abs(node1->getX() - node2->getX()) + abs(node1->getY() - node2->getY()));
	}

	vector<Connector>& getConnectors() { return connectors; }
	const vector<Connector*>& getForbiddenConnectors() const noexcept {
		return forbiddenConnector;
	}
	const vector<Connector*>& getWallConnectors() const noexcept {
		return wallConnector;
	}
	void addForbiddenConnector(Connector* co) {
		forbiddenConnector.push_back(co);
	}

	void popInvalidConnectors() noexcept;

	vector<const Connector*> getPath(int beginId, int goalId);
	vector<const Connector*> getBestUnkown(int startId);

	map<unsigned int, ObjectInfo> getObjects() { return objects; };

	int getPressurePlatePosition(int ppId);
};


