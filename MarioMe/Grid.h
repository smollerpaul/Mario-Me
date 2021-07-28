#pragma once
#include "Cell.h"
#include "Utils.h"

class Cell;
class Grid
{
	Cell** grid = nullptr;

	int cellWidth = 0;
	int cellHeight = 0;
	int col = 0;
	int row = 0;

public:
	Grid(int col, int row, int cellW, int cellH);
	void Add(CGameObject* gObj, vector< D3DXVECTOR2> cells);
	void Clear();
	void FilterByCamera(Camera* cam, vector<LPGAMEOBJECT> objList, vector<LPGAMEOBJECT>& filteredList);

};

