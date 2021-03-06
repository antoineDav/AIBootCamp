#pragma once

#include "Node.h"
#include <map>
#include "TurnInfo.h"
#include "ObjectInfo.h"
#include <vector>
#include <algorithm>


using namespace std;

class Graph
{
private:
	int rowCount = 0;
	int colCount = 0;
	vector<Node> nodes {};
	vector<Connector> connectors {};
	vector<Connector*> forbiddenConnector {};
	vector<Connector*> wallConnector {};
	vector<Connector*> wallGrope {};
	map<unsigned int, ObjectInfo> objects {};


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

	//Mets � jour les infos des tiles et invalide les connectors � invalider
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
		int x = abs(node1->getX() - node2->getX());
		int y = abs(node1->getY() - node2->getY());
		int dist;
		(y > x) ? dist = y : dist = (x + y) / 2;
		return dist;
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
	
	void removeWallConnector(const Connector* co) {
		auto c = find_if(wallConnector.begin(), wallConnector.end(), [&co](Connector * it) {
			return it->getObjects() == co->getObjects();
		});
		wallConnector.erase(c);
	}

	vector<Connector*> getWallGrope() { return wallGrope; }
	void setWallGrope(Connector* connector) { wallGrope.push_back(connector); }

	void popInvalidConnectors() noexcept;
	void setVisitedNode(int id) { nodes[id].setVisited(); };
	vector<const Connector*> getPath(int beginId, int goalId); 
	vector<const Connector*> getNearUnkown(int startId);
	vector<const Connector*> getFarUnkown(int startId);
	vector<const Connector *> wallGroping(int startId);

	map<unsigned int, ObjectInfo> getObjects() { return objects; };

	int getPressurePlatePosition(int ppId);
};


