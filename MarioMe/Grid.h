#pragma once
#include "Cell.h"
#include "Utils.h"

class Cell;
class Grid
{
	Cell** grid = nullptr;

	int cellWidth = 48;
	int cellHeight = 48;
	int col = 0;
	int row = 0;

public:
	Grid(int col, int row, int cellW, int cellH);
	void Add(CGameObject* gObj, vector< D3DXVECTOR2> cells);

};

