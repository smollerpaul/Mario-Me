#include "PlayScene.h"
#include "Utils.h"
#include "Sprites.h"
#include "Portal.h"
#include "Camera.h"
#include "Map.h"
#include "SolidBlock.h"
#include "GhostBlock.h"
#include "QuestionBlock.h"

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
	player->Update(dt, &coObjects);

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	
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
		
	for (int i = 0; i < coObjects.size(); i++) {
		coObjects[i]->RenderBoundingBox();
	}
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

void CPlayScene::LoadMapObjects(string objectType, float x, float y, float width, float height)
{
	//mario spawn point
	if (objectType.compare("SpawnPoint") == 0) {
		player->SetPosition(x, y);
	}
	if (objectType.compare("SolidBlock") == 0) {
		SolidBlock* solidObject = new SolidBlock();
		solidObject->SetPosition(x,y);
		solidObject->SetSize(width, height);
		AddObject(solidObject);
	}
	if (objectType.compare("GhostBlock") == 0) {
		GhostBlock* ghostObject = new GhostBlock();
		ghostObject->SetPosition(x,y);
		ghostObject->SetSize(width, height);
		AddObject(ghostObject);
	}

	if (objectType.compare("Goomba") == 0) {
		CGoomba* goomba = new CGoomba();
		goomba->SetPositionBottom(x, y);
		AddObject(goomba);
	}

	if (objectType.compare("QuestionBlock") == 0) {
		QuestionBlock* qb = new QuestionBlock();
		qb->SetPosition(x, y);
		AddObject(qb);
	}


	// void, spawner, pipe
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



