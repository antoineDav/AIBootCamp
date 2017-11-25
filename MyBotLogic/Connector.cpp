#include "Connector.h"
#include "Node.h"
#include "GameManager.h"


void Connector::init(Node * _beginNode, Node * _endNode, Tile::ETilePosition _direction) 
{
	beginNode = _beginNode;
	endNode = _endNode;
	direction = _direction;
	isToDestroy = false;
}

bool Connector::hasDoor() const noexcept
{
	if (!hasObject())
	{
		return false;
	}
	else
	{
		std::set<Object::EObjectType> objectTypes = GameManager::get().getGraph().getObjects()[_objectId].objectTypes;
		for (Object::EObjectType obj : objectTypes) {
			if (obj == Object::ObjectType_Door)
			{
				return true;
			}
		}
		return false;
	}
}
