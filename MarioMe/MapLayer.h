#pragma once
#include "Utils.h"
#include "tinyXML/tinyxml.h"
class MapLayer
{
private:
	int width = 1;
	int height = 1;
	int visible = 1;

	int** tiles = nullptr;

public:
	MapLayer(TiXmlElement* node);

	bool IsVisible();

	int GetTileId(int x, int y);
};

