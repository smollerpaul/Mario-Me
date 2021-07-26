#include "Grid.h"
#include "Cell.h"

Grid::Grid( int col, int row, int cellW, int cellH)
{
	this->cellWidth = cellW;
	this->cellHeight = cellH;
	this->col = col;
	this->row = row;
}

void Grid::Add(CGameObject* gObj, vector< D3DXVECTOR2> cells)
{
	for (D3DXVECTOR2 cell : cells) {
		//grid[cell.x][cell.y].insert(gObj);

		DebugOut(L"( %f , %f ) inserted!\n", cell.x, cell.y);
	}

}
