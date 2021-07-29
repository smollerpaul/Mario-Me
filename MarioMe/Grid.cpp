#include "Grid.h"
#include "Cell.h"
#include "Camera.h"

Grid::Grid( int col, int row, int cellW, int cellH)
{
	this->cellWidth = cellW;
	this->cellHeight = cellH;
	this->col = col;
	this->row = row;

	grid = new Cell*[col];
	for (int i = 0; i < col; i++) {
		grid[i] = new Cell[row];
	}
}

void Grid::Add(CGameObject* gObj, vector< D3DXVECTOR2> cells)
{
	for (D3DXVECTOR2 cell : cells) {
		int cellX = (int)cell.x;
		int cellY = (int)cell.y;

		grid[cellX][cellY].AddObject(gObj);

		DebugOut(L"( %d , %d ) inserted!\n", cellX, cellY);
	}

}

void Grid::Clear()
{
	for (int i = 0; i < col; i++) {
		for (int j = 0; j < row; j++) {
			grid[i][j].Clear();
		}
	}
}

void Grid::FilterByCamera(Camera* cam, vector<LPGAMEOBJECT> objList, vector<LPGAMEOBJECT>& filteredList)
{
	
	int beginX = (int)floor(cam->GetX() / cellWidth);
	int beginY = (int)floor(cam->GetY() / cellHeight);
	int endX = (int)ceil(cam->GetX()+ cam->GetWidth() / cellWidth);
	int endY = (int)ceil(cam->GetY() + cam->GetHeight() / cellHeight);

	beginX = max(0, beginX);
	beginY = max(0, beginY);
	endX = min(col, endX);
	endY = min(row, endY);

	for (int i = beginX; i < endX; i++) {
		for (int j = beginY; j < endY; j++) {

			if (grid[i][j].cellObjects.size() < 1)
				continue;

			for each ( LPGAMEOBJECT obj in grid[i][j].cellObjects)
			{

				filteredList.push_back(obj);
			}
		}
	}
}
