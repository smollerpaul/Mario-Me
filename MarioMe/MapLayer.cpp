#include "MapLayer.h"

MapLayer::MapLayer(TiXmlElement* node)
{ 
	node->QueryIntAttribute("width", &width);
	node->QueryIntAttribute("height", &height);

	if (node->Attribute("visible")) {
		node->QueryIntAttribute("visible", &visible);
	}


	string data = node->FirstChildElement("data")->GetText();
	vector<string> splitted = split(data, ",");

	//declare a memory block of size
	tiles = new int* [this->width];

	for (int i = 0; i < this->width;i++) {
		tiles[i] = new int[height];

		for (int j = 0;j < this->height;j++) {
			int tileId = stoi(splitted[i + j * width]);
			tiles[i][j] = tileId;
		}
	}

}

bool MapLayer::IsVisible()
{
	return visible;
}

int MapLayer::GetTileId(int x, int y)
{
	if(x < 0 || x >= width) return 0;
	if (y < 0 || y >= height) return 0;

	return tiles[x][y];
}
