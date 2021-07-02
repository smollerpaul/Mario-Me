#pragma once
#include "MapEntry.h"

class Entries
{
public:
	unordered_map<int, MapEntry*> nodes;
	virtual void AddNode(MapEntry* node);
	virtual MapEntry* GetNode(int id);
};

