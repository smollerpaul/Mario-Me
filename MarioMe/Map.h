#pragma once
#include "Utils.h"
#include "MapLayer.h"

class GameMap
{
#pragma region TileSet
protected:
	int firstgid = 1;
	int columns = 1;
	int tileWidth = 1;
	int tileHeight = 1;

	LPDIRECT3DTEXTURE9 tileImage = NULL;
#pragma endregion

	int width = 0;
	int height = 0;

	vector<MapLayer*> layers;

public:
	GameMap();
	void GetSize(int& mapWidth, int& mapHeight);

	static GameMap* Load(string path);

	void AddMapLayer(MapLayer* layer);

	void Render();
};
