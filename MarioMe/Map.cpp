#include "Map.h"
#include "TinyXML/tinyxml.h"
#include "Textures.h"
#include <stdlib.h>
#include "Camera.h"
#include "Game.h"

GameMap* GameMap::Load(string path)
{
	GameMap* result = new GameMap();

	TiXmlDocument doc(path.c_str());

	if (doc.LoadFile()) {
		TiXmlElement* root = doc.RootElement();

#pragma region TileSet
		TiXmlElement* tileSet = root->FirstChildElement("tileset");
		TiXmlElement* ctileImage = tileSet->FirstChildElement("image");

		tileSet->QueryIntAttribute("firstgid", &result->firstgid);
		tileSet->QueryIntAttribute("columns", &result->columns);
		tileSet->QueryIntAttribute("tilewidth", &result->tileWidth);
		tileSet->QueryIntAttribute("tileheight", &result->tileHeight);

		char mapDir[_MAX_DIR];
		_splitpath(path.c_str(), NULL, mapDir, NULL, NULL);
		string imgPath = (string)mapDir + ctileImage->Attribute("source");
		result->tileImage = CTextures::Load(ToLPCWSTR(imgPath), D3DCOLOR_ARGB(0, 0, 0, 0));
#pragma endregion

#pragma region Layer
		for (TiXmlElement* node = root->FirstChildElement("layer"); node != nullptr; node = node->NextSiblingElement("layer")) {
			result->layers.push_back(new MapLayer(node));
		}
#pragma endregion


		doc.Clear();
	}

	return result;
}

void GameMap::Render()
{
	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();

	int xBegin = max(camera->GetX() / tileWidth - 1, 0);
	int xEnd = (camera->GetX() + camera->GetWidth()) / tileWidth + 1;
	int yBegin = max(camera->GetY() / tileHeight - 1, 0);
	int yEnd = (camera->GetY() + camera->GetHeight()) / tileHeight + 1;

	for each (MapLayer * layer in layers)
	{
		if (!layer->IsVisible()) continue;

		for (int i = xBegin; i < xEnd; i++) {
			for (int j = yBegin; j < yEnd; j++) {

				int tileId = layer->GetTileId(i, j) - firstgid;
				if (tileId < 0) continue;

				int x = i * tileWidth - camera->GetX();
				int y = j * tileHeight - camera->GetY();

				int left = (tileId % columns) * tileWidth;
				int top = (tileId / columns) * tileHeight;
				int right = left + tileWidth;
				int bottom = top + tileHeight;

				CGame::GetInstance()->Draw(x, y, tileImage, left, top, right, bottom);
			}
		}
	}
}