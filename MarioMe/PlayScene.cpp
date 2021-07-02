#include "PlayScene.h"
#include "Utils.h"
#include "Sprites.h"
#include "Portal.h"
#include "Camera.h"
#include "Map.h"
#include "EffectVault.h"

using namespace std;

CPlayScene::CPlayScene(string id, string filePath):
	CScene(id, filePath)
{

}

void CPlayScene::Update(DWORD dt)
{

	vector<LPGAMEOBJECT> coObjects;

	coObjects.push_back(player);

	for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}
	//player update must be called separately
	// movement update 
	player->Update(dt);

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt);
	}

	player->CollisionUpdate(dt, &coObjects);

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->CollisionUpdate(dt, &coObjects);
	}

	player->BehaviorUpdate(dt);

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->BehaviorUpdate(dt);
	}

	player->ClearCollision();

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->ClearCollision();
	}

	camera->Update();
	EffectVault::GetInstance()->Update(dt);
	CheckAlive();

	PlayerData* pd = PlayerData::GetInstance();
	pd->UpdateGameTime(dt);

	DebugOut(L"[SCORE]: %d  [COINS]: %d  [Time left]: %f \n ", pd->GetScore(), pd->GetCoins(), pd->GetGameTime());
}

void CPlayScene::Render()
{
	this->map->Render();
	vector<LPGAMEOBJECT> coObjects;

	coObjects.push_back(player);
	
	for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}
	
	for (size_t i = 0; i < coObjects.size(); i++)
	{
		coObjects[i]->Render();
	}

	EffectVault::GetInstance()->Render();

	/*for (int i = 0; i < coObjects.size(); i++) {
		coObjects[i]->RenderBoundingBox();
	}*/
}

void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

CMario* CPlayScene::GetPlayer()
{
	return player;
}

void CPlayScene::OnKeyDown(int KeyCode)
{
	if (!player)
		return;

	player->OnKeyDown(KeyCode);
}

void CPlayScene::OnKeyUp(int KeyCode)
{
	if (!player)
		return;
	player->OnKeyUp(KeyCode);
}

void CPlayScene::Load()
{
	player = new CMario();

	DebugOut(L"[INFO] Start loading scene resources from : %s \n", ToLPCWSTR(sceneFilePath));

	TiXmlDocument doc(sceneFilePath.c_str());

	if (doc.LoadFile()) {
		TiXmlElement* root = doc.RootElement();

		TiXmlElement* tmxMap = root->FirstChildElement("TmxMap");
		string mapPath = tmxMap->Attribute("path");
		this->map = GameMap::Load(mapPath);

		/*TiXmlElement* cam = root->FirstChildElement("Camera");
		TiXmlElement* region = cam->FirstChildElement("Region");

		int regionId = region->QueryIntAttribute("id", &regionId);

		if (regionId.compare("0") == 0) {
			region->QueryFloatAttribute("left", &camStartX);
			region->QueryFloatAttribute("top", &camStartY);
		}
		*/
		
	/*	region->QueryFloatAttribute("right", &camWidth);
		region->QueryFloatAttribute("bottom", &camHeight);*/

		DebugOut(L"done cam playscene  %f  %f %f %f  \n", camStartX, camStartY, camWidth, camHeight);

		doc.Clear();
	}
	Camera* camera = new Camera();
	//camera->SetSize(camWidth, camHeight);
	//camera->SetPosition(camStartX, camStartY);
	camera->SetSize(700,480);
	camera->SetPosition(CAM_START_X, CAM_START_Y);
	SetCamera(camera);
	camera->SetFocusOnPlayer(player);

}

void CPlayScene::CheckAlive()
{
	// check for terminated objects & remove them
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->GetAlive()!=1)
			RemoveObject(objects[i]);
	}
}

int CPlayScene::GetSceneType()
{
	return SceneType;
}



