#include "Map.h"
#include "TinyXML/tinyxml.h"
#include "Textures.h"
#include "Utils.h"
#include "Camera.h"
#include "Game.h"
#include "GameObject.h"
#include "MapEntry.h"
#include "rapidjson/document.h"

using namespace rapidjson;

GameMap* GameMap::Load(string path)
{
	GameMap* result = new GameMap();
	TiXmlDocument doc(path.c_str());

	if (!doc.LoadFile()) {
		DebugOut(L" [ERROR MAP FILE] Map file failed to load!");
	}

	if (doc.LoadFile()) {
		TiXmlElement* root = doc.RootElement();

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

#pragma region Map
		root->QueryIntAttribute("width", &result->width);
		root->QueryIntAttribute("height", &result->height);

#pragma endregion

#pragma region Grid

		int columns = 0, rows = 0, cellW = 0, cellH = 0;
		TiXmlElement* mapProps = root->FirstChildElement("properties");

		if (mapProps != NULL) {

			for (TiXmlElement* node = mapProps->FirstChildElement("property"); node != nullptr; node = node->NextSiblingElement("property")) {
				string propName = node->Attribute("name");

				if (propName.compare("CellHeight") == 0) {
					node->QueryIntAttribute("value", &cellH);
				}
				if (propName.compare("CellWidth") == 0) {
					node->QueryIntAttribute("value", &cellW);
				}
				if (propName.compare("TotalColumn") == 0) {
					node->QueryIntAttribute("value", &columns);
				}
				if (propName.compare("TotalRow") == 0) {
					node->QueryIntAttribute("value", &rows);
				}

			}
			CGame::GetInstance()->GetCurrentScene()->LoadGrid(columns, rows, cellW, cellH);
		}
		

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
					string objName = objG->Attribute("name");

					if (objName.compare("MapObjects") == 0 || objName.compare("Spawners") == 0 ||
						objName.compare("Object Layer") == 0)
					{
						string objType = obj->Attribute("type");

						float x = 0, y = 0;
						float objWidth = 0, objHeight = 0;
						vector<D3DXVECTOR2> cells;

						if (obj->Attribute("x") != NULL)
							obj->QueryFloatAttribute("x", &x);

						if (obj->Attribute("y") != NULL)
							obj->QueryFloatAttribute("y", &y);

						if (obj->Attribute("width") != NULL)
							obj->QueryFloatAttribute("width", &objWidth);

						if (obj->Attribute("height") != NULL)
							obj->QueryFloatAttribute("height", &objHeight);

						//grid

						TiXmlElement* props = obj->FirstChildElement("properties");
						
						if (props != NULL) {
							for (TiXmlElement* gridProp = props->FirstChildElement("property"); gridProp != nullptr; gridProp = gridProp->NextSiblingElement("property")) {
								string propName = gridProp->Attribute("name");

								if (propName.compare("Grid") == 0) {
									Document doc;
									doc.Parse(gridProp->Attribute("value"));

									for (auto& v : doc.GetArray()) {
										auto cr = v.GetObjectW();
										int x = cr["x"].GetInt();
										int y = cr["y"].GetInt();

										D3DXVECTOR2 cell = D3DXVECTOR2(x, y);
										cells.push_back(cell);
									}
									doc.Clear();
								}
							}

						}
						
						//Read special objs
						if (objType.compare("QuestionBlock") == 0) {
							TiXmlElement* props = obj->FirstChildElement("properties");

							for (TiXmlElement* objProp = props->FirstChildElement("property"); objProp != nullptr; objProp = objProp->NextSiblingElement("property")) {
								string propName = objProp->Attribute("name");

								if (propName.compare("HiddenItem") == 0) {
									string propValue = objProp->Attribute("value");
									CGame::GetInstance()->GetCurrentScene()->LoadMapObjects(propValue, cells, x, y, objWidth, objHeight);
								}
							}
						}

						if (objType.compare("Pipe") == 0) {
							TiXmlElement* props = obj->FirstChildElement("properties");

							int length, type, direction;
							for (TiXmlElement* objProp = props->FirstChildElement("property"); objProp != nullptr; objProp = objProp->NextSiblingElement("property")) {
								string propName = objProp->Attribute("name");

								if (propName.compare("Length") == 0)
									objProp->QueryIntAttribute("value", &length);
								if (propName.compare("Type") == 0)
									objProp->QueryIntAttribute("value", &type);
								if (propName.compare("Direction") == 0)
									objProp->QueryIntAttribute("value", &direction);
							}
							CGame::GetInstance()->GetCurrentScene()->LoadSpecialObject(objType, cells, x, y, objWidth, objHeight, length, type, direction);
						}

						if (objType.compare("Spawner") == 0) {
							TiXmlElement* props = obj->FirstChildElement("properties");

							for (TiXmlElement* objProp = props->FirstChildElement("property"); objProp != nullptr; objProp = objProp->NextSiblingElement("property")) {
								string propName = objProp->Attribute("name");

								if (propName.compare("EntityType") == 0) {
									string propValue = objProp->Attribute("value");
									CGame::GetInstance()->GetCurrentScene()->LoadMapObjects(propValue, cells, x, y, objWidth, objHeight);
								}
							}
						}

						if (objType.compare("EndPortal") == 0) {
							TiXmlElement* props = obj->FirstChildElement("properties");
							int direct = 0;
							for (TiXmlElement* objProp = props->FirstChildElement("property"); objProp != nullptr; objProp = objProp->NextSiblingElement("property")) {
								string propName = objProp->Attribute("name");

								if (propName.compare("Direction") == 0) {
									objProp->QueryIntAttribute("value", &direct);
									//DebugOut(L"end dir %d \n", direct);
								}
							}
							CGame::GetInstance()->GetCurrentScene()->LoadSpecialObject(objType, cells, x, y, objWidth, objHeight, 0, 0, 0, 0, 0, direct, 0);
						}

						if (objType.compare("BeginPortal") == 0) {
							TiXmlElement* props = obj->FirstChildElement("properties");

							float desX = 0, desY = 0;
							int dir = 0, targetReg = -1;
							for (TiXmlElement* objProp = props->FirstChildElement("property"); objProp != nullptr; objProp = objProp->NextSiblingElement("property")) {
								string propName = objProp->Attribute("name");

								if (propName.compare("DestinationX") == 0) {
									objProp->QueryFloatAttribute("value", &desX);
								}

								if (propName.compare("DestinationY") == 0) {
									objProp->QueryFloatAttribute("value", &desY);
								}

								if (propName.compare("Direction") == 0) {
									objProp->QueryIntAttribute("value", &dir);
								}

								if (propName.compare("CameraRegion") == 0) {
									objProp->QueryIntAttribute("value", &targetReg);
								}
							}
							CGame::GetInstance()->GetCurrentScene()->LoadSpecialObject(objType, cells, x, y, objWidth, objHeight, 0, 0, 0, desX, desY, dir, targetReg);
						}

						if (objType.compare("CloudPortal") == 0) {
							TiXmlElement* props = obj->FirstChildElement("properties");

							float desX = 0, desY = 0;
							int dir = 0, targetReg = -1;
							for (TiXmlElement* objProp = props->FirstChildElement("property"); objProp != nullptr; objProp = objProp->NextSiblingElement("property")) {
								string propName = objProp->Attribute("name");

								if (propName.compare("DestinationX") == 0) {
									objProp->QueryFloatAttribute("value", &desX);
								}

								if (propName.compare("DestinationY") == 0) {
									objProp->QueryFloatAttribute("value", &desY);
								}

								if (propName.compare("Direction") == 0) {
									objProp->QueryIntAttribute("value", &dir);
								}

								if (propName.compare("CameraRegion") == 0) {
									objProp->QueryIntAttribute("value", &targetReg);
								}
							}
							CGame::GetInstance()->GetCurrentScene()->LoadSpecialObject(objType, cells, x, y, objWidth, objHeight, 0, 0, 0, desX, desY, dir, targetReg);
						}

						// read next
						CGame::GetInstance()->GetCurrentScene()->LoadMapObjects(objType, cells, x, y, objWidth, objHeight);
					}

					if (objName.compare("WorldGraph") == 0) {

						float xPos = 0, yPos = 0;
						obj->QueryFloatAttribute("x", &xPos);
						obj->QueryFloatAttribute("y", &yPos);

						string adjList, adjWeight, uncheckedSprite, checkedSprite, sceneID;
						int worldNumber = 0, nodeID = 0;
						bool isStartPos = false;

						TiXmlElement* props = obj->FirstChildElement("properties");
						for (TiXmlElement* objProp = props->FirstChildElement("property"); objProp != nullptr; objProp = objProp->NextSiblingElement("property")) {
							string propName = objProp->Attribute("name");

							if (propName.compare("adjacent_list") == 0) {
								adjList = objProp->Attribute("value");
								//DebugOut(L" adjList: %s \n", ToLPCWSTR(adjList));
							}
							if (propName.compare("adjacent_weight") == 0) {
								adjWeight = objProp->Attribute("value");
								//DebugOut(L" adjWeight: %s \n", ToLPCWSTR(adjWeight));
							}
							if (propName.compare("node_id") == 0) {
								objProp->QueryIntAttribute("value", &nodeID);
								//DebugOut(L"3\n");
							}
							if (propName.compare("scene") == 0) {
								sceneID = objProp->Attribute("value");
								//DebugOut(L"4\n");
							}
							if (propName.compare("sprite_checked") == 0) {
								checkedSprite = objProp->Attribute("value");
								//DebugOut(L"5\n");
							}
							if (propName.compare("sprite_uncheck") == 0) {
								uncheckedSprite = objProp->Attribute("value");
								//DebugOut(L"5\n");
							}
							if (propName.compare("world_number") == 0) {
								objProp->QueryIntAttribute("value", &worldNumber);
								//DebugOut(L"6\n");
							}
							if (propName.compare("Start") == 0) {
								objProp->QueryBoolAttribute("value", &isStartPos);
								//DebugOut(L"7\n");
							}

							//DebugOut(L"8\n");
						}
						CGame::GetInstance()->GetCurrentScene()->LoadMapEntries(xPos, yPos, nodeID, sceneID, checkedSprite, uncheckedSprite, adjList, adjWeight, worldNumber, isStartPos);


					}

					if (objName.compare("AnimatedBG") == 0) {
						float xPos = 0, yPos = 0;
						obj->QueryFloatAttribute("x", &xPos);
						obj->QueryFloatAttribute("y", &yPos);

						CGame::GetInstance()->GetCurrentScene()->LoadBackground(xPos, yPos);
					}

				}

			}

		}
#pragma endregion

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

				CGame::GetInstance()->Draw(x + tileWidth / 2, y + tileHeight / 2, D3DXVECTOR3(0, 0, 0), tileImage, left, top, right, bottom);
			}
		}
	}
}

GameMap::GameMap()
{
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
