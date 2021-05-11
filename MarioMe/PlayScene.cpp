
#include "PlayScene.h"
#include "Utils.h"
#include "Sprites.h"
#include "Portal.h"
#include "Camera.h"
#include "Map.h"
#include "SolidBlock.h"
#include "GhostBlock.h"


using namespace std;

CPlayScene::CPlayScene(string id, string filePath):
	CScene(id, filePath)
{

}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

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

}

void CPlayScene::Render()
{
	this->map->Render();
	vector<LPGAMEOBJECT> coObjects;

	//add player
	
	coObjects.push_back(player);
	
	
	/*for (size_t i = 0; i < objects.size(); i++)
	{
		
		coObjects.push_back(objects[i]);
	}*/
	
 // de y cai phan tu mang ( starts at 0)

	for (size_t i = 0; i < coObjects.size(); i++)
	{
		//DebugOut(L"stopppppppp size %d\n", coObjects.size());
		coObjects[i]->Render();
	}
		
	/*for (int i = 0; i < objects.size(); i++)
		objects[i]->RenderBoundingBox();*/
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
	camera->SetPosition(500,900);
	SetCamera(camera);
	camera->SetFocusOnPlayer(player);

}

void CPlayScene::LoadMapObjects(string objectType, float x, float y, float width, float height)
{
	if (objectType.compare("SpawnPoint") == 0) {
		player->SetPosition(x, y);
		
	}
	if (objectType.compare("SolidBlock") == 0) {
		SolidBlock* solidObject = new SolidBlock();
		solidObject->SetPosition(x,y);
		solidObject->SetSize(width, height);
		AddObject(solidObject);

		/*float ww, hh, px, py;
		solidObject->GetPosition(px, py);
		solidObject->GetSize(ww, hh);
		DebugOut(L"oooooo %f, %f\n", px, py);
		DebugOut(L"eeeeeee  w %f, h %f \n", ww,hh);*/
	}
	if (objectType.compare("GhostBlock") == 0) {
		GhostBlock* ghostObject = new GhostBlock();
		ghostObject->SetPosition(x,y);
		ghostObject->SetSize(width, height);
		AddObject(ghostObject);
	}

	// void, spawner, pipe
}



