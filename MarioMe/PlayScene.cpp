#include "PlayScene.h"
#include "Utils.h"
#include "Sprites.h"
#include "Portal.h"
#include "Camera.h"
#include "Map.h"


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

	CheckAlive();
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
	player->OnKeyDown(KeyCode);

}

void CPlayScene::OnKeyUp(int KeyCode)
{
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

		doc.Clear();
	}
	Camera* camera = new Camera();
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



