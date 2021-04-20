
#pragma once
#include "Utils.h"
#include "MapLayer.h"


class GameMap
{
#pragma region TileSet
private:

	int firstgid = 1;
	int columns = 1;
	int tileWidth = 1;
	int tileHeight = 1;

	LPDIRECT3DTEXTURE9 tileImage;

#pragma endregion
	
	vector<MapLayer*> layers;

public:
	static GameMap* Load(string path);

	virtual void Render();
};
