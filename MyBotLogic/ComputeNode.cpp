#include "ComputeNode.h"

ComputeNode::ComputeNode(Node * originalNode)
{
	node = originalNode;
	entry = nullptr;
	exit = nullptr;
}

ComputeNode::ComputeNode(Node * originalNode, Node * dest)
{
	node = originalNode;
	calcEstim(dest);
	entry = nullptr;
	exit = nullptr;
}
