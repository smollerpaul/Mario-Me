#include "Entries.h"
void Entries::AddNode(MapEntry* node)
{
	if (node == nullptr)
		return;

	this->nodes[node->nodeId] = node ;
}

MapEntry* Entries::GetNode(int id)
{
	if (this->nodes.find(id) == this->nodes.end()) 
		return nullptr;

	return this->nodes.at(id);
}
