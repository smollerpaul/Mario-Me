
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



//void CPlayScene::_ParseSection_OBJECTS(string line)
//{
//	vector<string> tokens = split(line);
//
//	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());
//
//	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y
//
//	int object_type = atoi(tokens[0].c_str());
//	float x = atof(tokens[1].c_str());
//	float y = atof(tokens[2].c_str());
//
//	int ani_set_id = atoi(tokens[3].c_str());
//
//	CGameObject* obj = NULL;
//
//	switch (object_type)
//	{
//	case OBJECT_TYPE_MARIO:
//		if (player != NULL)
//		{
//			DebugOut(L"[ERROR] MARIO object was created before!\n");
//			return;
//		}
//		obj = new CMario(x, y);
//		player = (CMario*)obj;
//
//		DebugOut(L"[INFO] Player object created!\n");
//		break;
//	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
//	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
//	case OBJECT_TYPE_KOOPAS: obj = new CKoopas(); break;
//	case OBJECT_TYPE_PORTAL:
//	{
//		float r = atof(tokens[4].c_str());
//		float b = atof(tokens[5].c_str());
//		int scene_id = atoi(tokens[6].c_str());
//		obj = new CPortal(x, y, r, b, scene_id);
//	}
//	break;
//	default:
//		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
//		return;
//	}
//
//	// General object setup
//	obj->SetPosition(x, y);
//
//	//LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
//
//	//obj->SetAnimationSet(ani_set);
//	objects.push_back(obj);
//}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n",ToLPCWSTR(sceneFilePath));

	TiXmlDocument doc(sceneFilePath.c_str());

	if (doc.LoadFile()) {
		TiXmlElement* root = doc.RootElement();

		TiXmlElement* tmxMap = root->FirstChildElement("TmxMap");
		string mapPath = tmxMap->Attribute("path");
		this->map = GameMap::Load(mapPath);

		this->camera = new Camera();
		this->camera->SetPosition(0, 0);
		//this->camera->SetSize(2000, 579);
		this->camera->SetSize(500, 250);

		doc.Clear();
	}
	
	/*Camera* camera = new Camera();
	SetCamera(camera);

	camera->SetFocusOnPlayer(player);*/

}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

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

	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
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

void CPlayScene::LoadMapObjects(string objectName, float x, float y, float width, float height)
{
	/*if (objectName.compare("SolidBlock") == 0) {
		SolidBlock* solidObject = new SolidBlock();
		solidObject->SetPosition(x,y);
		solidObject->SetSize(width, height);
		AddObject(solidObject);
	}
	if (objectName.compare("GhostBlock") == 0) {
		GhostBlock* ghostObject = new GhostBlock();
		ghostObject->SetPosition(x,y);
		ghostObject->SetSize(width, height);
		AddObject(ghostObject);
	}*/
	DebugOut(L"elooooooo");
}



