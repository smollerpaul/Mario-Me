#pragma once
#include "Utils.h"
#include "GameObject.h"

class Cell
{
public:
	set<LPGAMEOBJECT> cellObjects;

	void AddObject(CGameObject* obj);
};

