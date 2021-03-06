#include "WorldScene.h"
#include "Utils.h"
#include "Map.h"
#include "Camera.h"
#include "MapEntry.h"
#include "OverworldTrees.h"
#include "PlayScene.h"

//switch scene faulty
WorldScene::WorldScene(string id, string filePath) :CScene(id, filePath)
{

}

void WorldScene::Load()
{
	player = new MapMario();
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", ToLPCWSTR(sceneFilePath));
	Camera* camera = new Camera();
	TiXmlDocument doc(sceneFilePath.c_str());

	if (doc.LoadFile()) {
		TiXmlElement* root = doc.RootElement();

		TiXmlElement* tmxMap = root->FirstChildElement("TmxMap");
		string mapPath = tmxMap->Attribute("path");
		this->map = GameMap::Load(mapPath);

		TiXmlElement* cam = root->FirstChildElement("Camera");
		TiXmlElement* region = cam->FirstChildElement("Region");

		region->QueryFloatAttribute("left", &camStartX);
		region->QueryFloatAttribute("top", &camStartY);
		region->QueryFloatAttribute("right", &camWidth);
		region->QueryFloatAttribute("bottom", &camHeight);

		doc.Clear();
	}

	camera->SetSize(camWidth, camHeight);
	camera->SetViewPort(0, 0, CAM_WIDTH_SIZE, CAM_HEIGHT_SIZE);
	//camera->SetPosition(camStartX, camStartY);
	SetCamera(camera);
}

void WorldScene::Update(DWORD dt)
{
	player->Update(dt);
	//EffectVault::GetInstance()->Update(dt);
}

void WorldScene::Render()
{
	CGame::GetInstance()->GetDirect3DDevice()->Clear(0, NULL, D3DCLEAR_TARGET, OVERWORLD_COLOR, 1.0f, 0);

	this->map->Render();
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Render();
	}
	EffectVault::GetInstance()->Render();

	//render last
	player->Render();

}

void WorldScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	EffectVault::GetInstance()->ClearEffects();
	player = nullptr;

	DebugOut(L"[INFO] Scene %s unloaded! \n", ToLPCWSTR(sceneFilePath));
}

void WorldScene::LoadMapObjects(string objectType, vector< D3DXVECTOR2> cells, float x, float y, float width, float height)
{
}

void WorldScene::LoadMapEntries(float xPos, float yPos, int nodeID, string sceneID, string checkedSprite, string uncheckedSprite, string adjList, string adjWeight, int worldNumber, bool isStartPos)
{
	MapEntry* mapE = new MapEntry(xPos, yPos, nodeID, sceneID, checkedSprite, uncheckedSprite,
		adjList, adjWeight, worldNumber, isStartPos);

	player->AddNode(mapE);
	AddObject(mapE);
}

void WorldScene::LoadBackground(float xPos, float yPos)
{
	EffectVault::GetInstance()->AddEffect(new OverworldTrees(xPos, yPos));
}

MapMario* WorldScene::GetPlayer()
{
	return player;
}

void WorldScene::OnKeyDown(int KeyCode)
{
	if (!player)
		return;
	player->OnKeyDown(KeyCode);
}

void WorldScene::OnKeyUp(int KeyCode)
{
	if (!player)
		return;
	player->OnKeyUp(KeyCode);
}

int WorldScene::GetSceneType()
{
	return SceneType;
}
