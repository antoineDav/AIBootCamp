#include "Graph.h"
#include <algorithm>
#include "Utility.h"
#include "GameManager.h"

#include "Debug.h"

int Graph::getPositionId(int x, int y) const noexcept {
	if ((x >= 0) && (x < colCount*2) && (y >= 0) && (y < rowCount))
		return x / 2 + colCount * y;
	return -1;
}

void Graph::tryAddConnector(Node& node, Tile::ETilePosition dir, int x, int y, int& index) noexcept {
	int i = getPositionId(x, y);
	if (i != -1) {
		connectors[index].init(&node, &nodes[i], dir);
		node.addConnector(&connectors[index]);
		++index;
	}
}

//Create the nodes without their connectors with the Node constructor
void Graph::createNodes() noexcept {
	for (int i = 0; i < colCount * rowCount; ++i) {
		nodes.push_back(Node{ (unsigned int) i, colCount });
	}
}

//Create all connectors empty
void Graph::createConnectors() noexcept {
	for (int i = 0; i < (((rowCount * (colCount - 1)) + ((2 * colCount - 1)*(rowCount - 1)))) * 2; ++i) {
		connectors.push_back(Connector{});
	}
}

//Create the connectors for each accessible neighbours for each node
void Graph::initConnectors() noexcept {
	int i{};
	for_each(nodes.begin(), nodes.end(), [&](Node& node) {
		int x{ node.getX() };
		int y{ node.getY() };
		tryAddConnector(node, Tile::NW, x - 1, y - 1, i);
		tryAddConnector(node, Tile::NE, x + 1, y - 1, i);
		tryAddConnector(node, Tile::W, x - 2, y, i);
		tryAddConnector(node, Tile::E, x + 2, y, i);
		tryAddConnector(node, Tile::SW, x - 1, y + 1, i);
		tryAddConnector(node, Tile::SE, x + 1, y + 1, i);
	});
}

void Graph::init(int _rowCount, int _colCount, const std::map<unsigned int, TileInfo>& tiles, const std::map<unsigned int, ObjectInfo>& objects) noexcept {
	rowCount = _rowCount;
	colCount = _colCount;
	createNodes();
	createConnectors();
	initConnectors();
	update(tiles, objects);
	popInvalidConnectors();
}

void Graph::updateNodesType(const std::map<unsigned int, TileInfo>& tiles) noexcept {
	for_each(tiles.begin(), tiles.end(), [&](const std::pair<unsigned int, TileInfo>& tile) {
		if (tile.second.tileType == Tile::TileAttribute_Goal && nodes[tile.second.tileID].getType() != Tile::TileAttribute_Goal) {
			GameManager::get().returnObjective(tile.second.tileID);
		}
		nodes[tile.second.tileID].setType(tile.second.tileType);
#ifdef DEBUGBOT_GRAPH
		file << "Node id : " << tile.second.tileID << " - "  << endl;
		const map<Tile::ETileType, const char*> tileTypeString{
			{ Tile::ETileType::TileAttribute_Default , "Default" },
			{ Tile::ETileType::TileAttribute_Goal, "Goal" },
			{ Tile::ETileType::TileAttribute_Forbidden, "Forbidden" },
			{ Tile::ETileType::TileAttribute_Omniscient, "Omniscient" },
			{ Tile::ETileType::TileAttribute_Unknown, "Unknown" }
		};
		const map<Tile::ETilePosition, const char*> tilePositionString{
			{ Tile::ETilePosition::NE , "NE" },
			{ Tile::ETilePosition::E, "E" },
			{ Tile::ETilePosition::SE, "S" },
			{ Tile::ETilePosition::SW, "SW" },
			{ Tile::ETilePosition::W, "W" },
			{ Tile::ETilePosition::NW, "NW" },
			{ Tile::ETilePosition::CENTER, "CENTER" }
		};
		file << "\tType : " << tileTypeString.find(tile.second.tileType)->second << endl;
		std::vector<Connector*>* connectors = GameManager::get().getGraph().getNode(tile.first).getAvailableConnectors();
		for (auto connector : *connectors){
			file << "\tConnector : " << connector->getBeginNode()->getId() << " -> " << connector->getEndNode()->getId() << endl;
			file << "\t\t\tDirection : " << tilePositionString.find(connector->getDirection())->second << endl;
			file << "\t\t\tObject Id : " << connector->getObjects() << endl;
			file << "\t\t\tIs Destroy : " << connector->getIsToDestroy() << endl;
		}
#endif // DEBUGBOT_GRAPH
	});
}

void Graph::updateNodeObjects(const std::map<unsigned int, ObjectInfo>& in_objects)
{
	for_each(in_objects.begin(), in_objects.end(), [&](const std::pair<unsigned int, ObjectInfo>& in_object) {
		objects[in_object.second.objectID] = in_object.second;
		auto i = find(in_object.second.objectTypes.begin(), in_object.second.objectTypes.end(), Object::EObjectType::ObjectType_PressurePlate);
		if (i != in_object.second.objectTypes.end())
		{
			nodes[in_object.second.tileID].setObject(in_object.second.objectID);
		}
		else
		{
			Connector* connector = nodes[in_object.second.tileID].getAvailableConnector(in_object.second.position);

			if (connector && !connector->hasObject()) { //No connector if wall on outer edge of map

				connector->setObject(in_object.second.objectID);

				//Connector du Node a l'autre bout de la connexion
				connector = connector->getEndNode()->getAvailableConnector(connector->getInvertDirection());
				connector->setObject(in_object.second.objectID);

				//Mark zone as dirty if new object potentially separates a zone in two
				Node* beginNode = connector->getBeginNode();
				Node* endNode = connector->getEndNode();

			}
		}
	});
}

void Graph::updateConnectorsWithType(const std::map<unsigned int, TileInfo>& tiles) noexcept {
	for_each(tiles.begin(), tiles.end(), [&](const std::pair<unsigned int, TileInfo>& tile) {
		Node& node = nodes[tile.second.tileID];
		if (node.getType() == Tile::TileAttribute_Forbidden) {
			vector<Connector*>* connectors{ node.getConnectors() };
			for_each(connectors->begin(), connectors->end(), [&](Connector* connector) {
				Connector* c1 = connector->getEndNode()->getConnector(connector->getBeginNode());
				if (c1 != nullptr) {
					c1->setIsToDestroy(true);
					forbiddenConnector.push_back(c1);
				}
				connector->setIsToDestroy(true);
				forbiddenConnector.push_back(connector);
			});
		}
		else if ((node.getType() != Tile::TileAttribute_Forbidden) && (tile.second.tileType == Tile::TileAttribute_Forbidden)) {
			node.setType(tiles.find(node.getId())->second.tileType);
			//Pop old connectors for the neighbours of the node
			vector<Connector*>* connectors{ node.getConnectors() };
			for_each(connectors->begin(), connectors->end(), [](Connector* connector) {
				connector->getEndNode()->popConnector(connector->getBeginNode());
			});
			//Clear connectors for the node
			node.clearConnectors();
		}
		node.setType(tile.second.tileType);
	});	
}
void Graph::updateConnectorsWithObjects(const std::map<unsigned int, ObjectInfo>& objects) noexcept {
	for_each(objects.begin(), objects.end(), [&](const std::pair<unsigned int, ObjectInfo>& object) {
		const std::set<Object::EObjectType>& objectTypes = object.second.objectTypes;
		if ((objectTypes.find(Object::ObjectType_Wall) != objectTypes.end()) || (objectTypes.find(Object::ObjectType_Window) != objectTypes.end())) {
			Connector* c1 = nodes[object.second.tileID].getAvailableConnector(object.second.position);
			if ((c1 != nullptr) && (c1->getIsToDestroy() == false)) {
				c1->setIsToDestroy(true);
				wallConnector.push_back(c1);
				Connector* c2 = c1->getEndNode()->getAvailableConnector(c1->getBeginNode());
				if ((c2 != nullptr) && (c2->getIsToDestroy() == false)) {
					c2->setIsToDestroy(true);
					wallConnector.push_back(c2);
				}
			}
		}
		for (Connector* wG : this->wallGrope)
		{
			if (wG->getObjects() == object.second.objectID && objectTypes.find(Object::ObjectType_Door) != objectTypes.end())
			{
				wG->getBeginNode()->popConnector(wG->getEndNode());
				wG->setIsToDestroy(false);
				wG->getBeginNode()->addAvailableConnector(wG);
				continue;
			}
		}
	});
	wallGrope.clear();
	
}

void Graph::update(const map<unsigned int, TileInfo>& tiles, const std::map<unsigned int, ObjectInfo>& objects) noexcept {
	updateNodesType(tiles);
	updateNodeObjects(objects);
	updateConnectorsWithType(tiles);
	updateConnectorsWithObjects(objects); 
	popInvalidConnectors();
}

class Graph::HeuristicManhattan {
private:
	const Node* goal;
public:
	HeuristicManhattan(const Node* _goal) noexcept
		: goal{ _goal } {}
	int operator()(const Node* node) const noexcept {
		int x = abs(goal->getX() - node->getX());
		int y = abs(goal->getY() - node->getY());
		int dist;
		(y > x) ? dist = y : dist = (x + y) / 2;
		return dist;
	}
};

struct Graph::NodeItem {
	Node* ptr;
	NodeItem* previous;
	const Connector* connector;
	int costSoFar;
	int estimatedTotalCost;
};

class Graph::NodeItemPtrComparison {
private:
	bool reverse;
public:
	NodeItemPtrComparison(const bool& revparam = false) : reverse{ revparam } {}
	bool operator() (NodeItem* _lnode, NodeItem* _rnode) const
	{
		if (reverse)
			return (_lnode->estimatedTotalCost < _rnode->estimatedTotalCost);
		else
			return (_lnode->estimatedTotalCost > _rnode->estimatedTotalCost);
	}
};

vector<const Connector*> Graph::getPath(int startId, int goalId) {
	Node* start{ &getNode(startId) };
	Node* end{ &getNode(goalId) };

	HeuristicManhattan heuristic{ &getNode(goalId)};

	//Initialize the record for the start node
	NodeItem *startRecord = new NodeItem{};
	startRecord->ptr = start;
	startRecord->previous = nullptr;
	startRecord->connector = nullptr;
	startRecord->costSoFar = 0;
	startRecord->estimatedTotalCost = heuristic(start);

	vector<NodeItem*> closedList;
	map<NodeItem*, int> openList;

	openList.insert(make_pair( startRecord, startRecord->estimatedTotalCost ));
	NodeItem* current;
	while (openList.size() > 0) {
		auto min = min_element(openList.begin(), openList.end(), [](pair<NodeItem*, int> left, pair<NodeItem*, int> right) -> bool
		{
			return (left.second < right.second);
		});
		current = min->first;
		openList.erase(min->first);

		//If it is the goal node, then terminate
		if (*current->ptr == *end) {
			break;
		}

		//Otherwise get its outgoing connections
		vector<Connector*>* neighbours = current->ptr->getAvailableConnectors();

		for (auto& neighbour : *neighbours) {
			const Node* neighbourNode = neighbour->getEndNodeC();
			int neighbourNodeCost = current->costSoFar + 1;

			NodeItem* neighbourRecord = new NodeItem();
			int neighbourNodeHeuristic;

			//Here we find the record in the open list corresponding to the neighbourNode if it exist
			auto neighbourRecordOpen = find_if(openList.begin(), openList.end(), [&neighbour](pair<NodeItem*, int> p) ->bool {
				return (*p.first->ptr == *neighbour->getEndNodeC());
			});
			//Here we find the record in the closed list corresponding to the neighbourNode if it exist
			std::vector<NodeItem*>::iterator neighbourRecordClose = std::find_if(closedList.begin(), closedList.end(), [&neighbour](NodeItem* ni) -> bool {
				return (*ni->ptr == *neighbour->getEndNodeC());
			});

			//If the node is closed we may have to skip, or remove it from the closed list
			if (neighbourRecordClose != closedList.end()) {
				neighbourRecord = *neighbourRecordClose;

				//if we didn't find a shorter route, skip
				if (neighbourRecord->costSoFar <= neighbourNodeCost)
					continue; 

				//otherwise remove it from the closed list
				closedList.erase(neighbourRecordClose);
				neighbourNodeHeuristic = neighbourRecord->estimatedTotalCost - neighbourRecord->costSoFar;
			}

			//Skip if the node is open and we've not found a better route
			else if 
				(neighbourRecordOpen != openList.end()) {
				
				neighbourRecord = neighbourRecordOpen->first;
				//If our route is no better, then skip
				if (neighbourRecord->costSoFar <= neighbourNodeCost)
					continue;
				neighbourNodeHeuristic = neighbourRecord->estimatedTotalCost - neighbourRecord->costSoFar;
			}

			//Otherwise we know we've got an unvisited node, so make a record for it
			else {
				neighbourRecord->ptr = neighbour->getEndNode();
				neighbourNodeHeuristic = heuristic(neighbourRecord->ptr);
				neighbourRecord->estimatedTotalCost = neighbourNodeCost + neighbourNodeHeuristic;
			}

			neighbourRecord->previous = current;
			neighbourRecord->connector = neighbour;
			neighbourRecord->costSoFar = neighbourNodeCost;
			if (neighbourRecordOpen == openList.end()) {
				openList.insert(make_pair(neighbourRecord, neighbourRecord->estimatedTotalCost));
			}
		}

		closedList.push_back(current);
	}


	vector<const Connector*> path;

	if (*current->ptr != *end)
		// FAILURE
		return path; 
	else {
		//Work back along the path, accumulating connections
		while (*current->ptr != *start) {
			path.push_back(current->connector);
			current = current->previous;
		}
	}

	//We liberate all the memory allocate for the NodeRecord* in the closedList and in the openList
	std::for_each(closedList.begin(), closedList.end(), [](NodeItem* ni) {
		ni->previous = nullptr;
		delete(ni);
	});

	std::for_each(openList.begin(), openList.end(), [](pair<NodeItem*, int> p) {
		p.first->previous = nullptr;
		delete(p.first);
	});
	
	// SUCCESS
	return path;
}


vector<int> Graph::getGoalPosition() const noexcept
{
	vector<int> result;
	for_each(nodes.begin(), nodes.end(), [&result](const Node& node) {
		if (node.isGoal())
			result.push_back(node.getId());
	});
	return result;
}

void Graph::popInvalidConnectors() noexcept {
	for_each(forbiddenConnector.begin(), forbiddenConnector.end(), [&](Connector* connector) {
		connector->getBeginNode()->popConnector(connector->getEndNode());
		connector->getBeginNode()->popAvailableConnector(connector->getEndNode());
	});
	for_each(wallConnector.begin(), wallConnector.end(), [&](Connector* connector) {
		connector->getBeginNode()->popAvailableConnector(connector->getEndNode());
	});
	forbiddenConnector.clear();
}

vector<const Connector*> Graph::getFarUnkown(int startId) {
	Node* start{ &getNode(startId) };

	NodeItem *startRecord = new NodeItem{};
	startRecord->ptr = start;
	startRecord->previous = nullptr;
	startRecord->connector = nullptr;
	startRecord->costSoFar = 0;

	vector<NodeItem*> closedList;
	MyPriorityQueue<NodeItem*, std::vector<NodeItem*>, NodeItemPtrComparison> openList;

	openList.push(startRecord);
	NodeItem* current;
	//Iterate through processing each node
	while (openList.size() > 0) {
		current = openList.top();
		openList.pop();
		
		//If it is an unknown node, then terminate
		if (current->ptr->getType() == Tile::ETileType::TileAttribute_Unknown) {
			break;
		}

		vector<Connector*>* connections = current->ptr->getAvailableConnectors();
		NodeItem* connectionRecord;
		//Loop through each neighbours
		for (auto& connection : *connections) {
			connectionRecord = new NodeItem();
			const Node* endNode = connection->getEndNodeC();
			int endNodeCost = current->costSoFar + 1;

			std::vector<NodeItem*>::iterator connectionRecordClose = std::find_if(closedList.begin(), closedList.end(), [&connection](NodeItem* ni) -> bool {
				return (*ni->ptr == *connection->getEndNodeC());
			});
			std::vector<NodeItem*>::iterator connectionRecordOpen = std::find_if(openList.begin(), openList.end(), [&connection](NodeItem* ni) -> bool {
				return (*ni->ptr == *connection->getEndNodeC());
			});
			//If the node is closed we have to skip
			if (connectionRecordClose != closedList.end()) {
				continue;
			}
			else if(connectionRecordOpen != openList.end()) {
				connectionRecord = *connectionRecordOpen;
				//if we didn't find a shorter route, skip
				if (connectionRecord->costSoFar <= endNodeCost)
					continue;
			}
			else {
				connectionRecord->ptr = connection->getEndNode();
			}

			connectionRecord->previous = current;
			connectionRecord->connector = connection;
			connectionRecord->costSoFar = endNodeCost;
			connectionRecord->estimatedTotalCost = endNodeCost;
			//Add it to the open list
			if (connectionRecordOpen == openList.end()) {
				openList.push(connectionRecord);
			}
		}

		closedList.push_back(current);
	}

	vector<const Connector *> path;
	if (current->ptr->getType() != Tile::ETileType::TileAttribute_Unknown)
		// FAILURE
		return path;
	else {
		// SUCCESS
		while (*current->ptr != *start) {
			path.push_back(current->connector);
			current = current->previous;
		}
	}

	//We liberate all the memory
	std::for_each(closedList.begin(), closedList.end(), [](NodeItem* ni) {
		ni->previous = nullptr;
		delete(ni);
	});
	std::for_each(openList.begin(), openList.end(), [](NodeItem* ni) {
		ni->previous = nullptr;
		delete(ni);
	});
	return path;
}

vector<const Connector*> Graph::getNearUnkown(int startId) {
	vector<const Connector*> path = {};
	Node* start{ &getNode(startId) };
	int potential = 0;
	int previousPotential = 0;
	unsigned int plateId = -1;
	for (auto neighboursConnector : *start->getAvailableConnectors()) {
		Node* neighbor = neighboursConnector->getEndNode();
		if (neighbor->getVisited() == false) {
			if(neighbor->getObjects() >= 0){
				plateId = neighbor->getObjects();
			}
			for (auto neighborConnector : *neighbor->getConnectors()) {
				if (!neighborConnector->hasObject()) {
					potential += 2;
				}
				else {
					ObjectInfo obj = Graph::getObjects()[neighborConnector->getObjects()];
					if (find(obj.objectTypes.begin(), obj.objectTypes.end(),Object::ObjectType_Window)!= obj.objectTypes.end()) {
						potential += 6;
					}
					else if (find(obj.objectTypes.begin(), obj.objectTypes.end(), Object::ObjectType_Door)!= obj.objectTypes.end() ){
						if (find(obj.objectStates.begin(), obj.objectStates.end(), Object::EObjectState::ObjectState_Closed) != obj.objectStates.end() && obj.connectedTo.empty()) {
							potential += 3;
						}
						else if (find(obj.objectStates.begin(), obj.objectStates.end(), Object::EObjectState::ObjectState_Closed) != obj.objectStates.end() && find(obj.connectedTo.begin(), obj.connectedTo.end(),plateId)!= obj.connectedTo.end()) {
							potential += 10;
						}
						else if (find(obj.objectStates.begin(), obj.objectStates.end(), Object::EObjectState::ObjectState_Opened) != obj.objectStates.end() && neighborConnector->getBeginNode()->getVisited() == false) {
							potential += 15;
						}
						else {
							++potential;
						}
					}
				}
			}
			if (potential > previousPotential){
				path.clear();
				path.push_back(neighboursConnector);
				previousPotential = potential;
			}
			potential = 0;
			plateId = -1;
		}
	}
	//return one connector
	return path;
}

vector<const Connector *> Graph::wallGroping(int startId) {
	vector<const Connector*> path = {};
	Node* start{ &getNode(startId) };
	for (auto neighboursConnector : *start->getConnectors()) {
		if (neighboursConnector->getObjects() != -1 && neighboursConnector->getIsGrope() == false) {
			ObjectInfo obj = Graph::getObjects()[neighboursConnector->getObjects()];
			if (find(obj.objectTypes.begin(), obj.objectTypes.end(), Object::ObjectType_Wall) != obj.objectTypes.end()) {
				neighboursConnector->setIsGrope();
				path.push_back(neighboursConnector);
				return path;
			}
		}
	}
	if (path.empty())
	{
		for (auto neighboursConnector : *start->getAvailableConnectors()) {
			Node* neighbor{ neighboursConnector->getEndNode() };
			for (auto neighborConnector : *neighbor->getConnectors()) {
				if (neighborConnector->getObjects() != -1 && neighborConnector->getIsGrope() == false) {
					ObjectInfo obj = Graph::getObjects()[neighborConnector->getObjects()];
					if (find(obj.objectTypes.begin(), obj.objectTypes.end(), Object::ObjectType_Wall) != obj.objectTypes.end()) {
						path.push_back(neighboursConnector);
						return path;
					}
					if (find(obj.objectTypes.begin(), obj.objectTypes.end(), Object::ObjectType_Door) != obj.objectTypes.end()) {
						path.push_back(neighboursConnector);
						return path;
					}
				}
			}
		}
	}
	return path;
}
