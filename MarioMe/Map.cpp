#include "Map.h"
#include "TinyXML/tinyxml.h"
#include "Textures.h"
#include "Utils.h"
#include "Camera.h"
#include "Game.h"
#include "GameObject.h"

GameMap* GameMap::Load(string path)
{
	GameMap* result = new GameMap();
	TiXmlDocument doc(path.c_str());

	if (!doc.LoadFile()) {
		DebugOut(L" [ERROR MAP FILE] Map file failed to load!");
	}

	if (doc.LoadFile()) {
		TiXmlElement* root = doc.RootElement();

		
#pragma region Map
		root->QueryIntAttribute("width", &result->width);
		root->QueryIntAttribute("height", &result->height);
		/*map tile size is same as tileset tile size*/
#pragma endregion

#pragma region TileSet
		TiXmlElement* tileSet = root->FirstChildElement("tileset");
		TiXmlElement* tileSetImage = tileSet->FirstChildElement("image");

		tileSet->QueryIntAttribute("firstgid", &result->firstgid);
		tileSet->QueryIntAttribute("columns", &result->columns);
		tileSet->QueryIntAttribute("tilewidth", &result->tileWidth);
		tileSet->QueryIntAttribute("tileheight", &result->tileHeight);

		char mapDir[_MAX_DIR];
		_splitpath(path.c_str(), NULL, mapDir, NULL, NULL);
		string imgPath = (string)mapDir + tileSetImage->Attribute("source");
		result->tileImage = CTextures::Load(ToLPCWSTR(imgPath), D3DCOLOR_ARGB(0, 0, 0, 0));

#pragma endregion

#pragma region Layer
		for (TiXmlElement* node = root->FirstChildElement("layer"); node != nullptr; node = node->NextSiblingElement("layer")) {
			result->AddMapLayer(new MapLayer(node));
		}
#pragma endregion

#pragma region ObjectLayer

		for (TiXmlElement* objG = root->FirstChildElement("objectgroup"); objG != nullptr; objG = objG->NextSiblingElement("objectgroup")) {
			for (TiXmlElement* obj = objG->FirstChildElement("object"); obj != nullptr; obj = obj->NextSiblingElement("object")) {
				
				
				if (obj->Attribute("type") != NULL) {
					string objName= objG->Attribute("name");
					
					if (objName.compare("MapObjects") == 0) {

						string objType = obj->Attribute("type");
						float x = 0,  y = 0;
						float objWidth = 0, objHeight = 0;
						
						if( obj->Attribute("x")!=NULL)
							obj->QueryFloatAttribute("x", &x);

						if (obj->Attribute("y") != NULL)
							obj->QueryFloatAttribute("y", &y);

						if (obj->Attribute("width") != NULL)
							obj->QueryFloatAttribute("width", &objWidth);

						if (obj->Attribute("height") != NULL)
							obj->QueryFloatAttribute("height", &objHeight);

						CGame::GetInstance()->GetCurrentScene()->LoadMapObjects(objType,x, y, objWidth, objHeight);
					}
				}

			}
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

GameMap::GameMap()
{
	int firstgid = columns = tileWidth = tileHeight = 0;
	tileImage = NULL;
	int width= height = 0;
}

void GameMap::AddMapLayer(MapLayer* layer)
{
	layers.push_back(layer);
}

void GameMap::GetSize(int& mapWith, int& mapHeight)
{
	mapWith = width* tileWidth;
	mapHeight = height*tileHeight;
}
