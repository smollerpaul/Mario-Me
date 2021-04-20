#include <iostream>
#include <fstream>

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Camera.h"
#include "Map.h"

using namespace std;

CPlayScene::CPlayScene(string id, string filePath):
	CScene(id, filePath)
{

}

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6

#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024

//
//void CPlayScene::_ParseSection_TEXTURES(string line)
//{
//	vector<string> tokens = split(line);
//
//	if (tokens.size() < 5) return; // skip invalid lines
//
//	int texID = atoi(tokens[0].c_str());
//	wstring path = ToWSTR(tokens[1]);
//	int R = atoi(tokens[2].c_str());
//	int G = atoi(tokens[3].c_str());
//	int B = atoi(tokens[4].c_str());
//
//	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
//}

//void CPlayScene::_ParseSection_SPRITES(string line)
//{
//	vector<string> tokens = split(line);
//
//	if (tokens.size() < 6) return; // skip invalid lines
//
//	int ID = atoi(tokens[0].c_str());
//	int l = atoi(tokens[1].c_str());
//	int t = atoi(tokens[2].c_str());
//	int r = atoi(tokens[3].c_str());
//	int b = atoi(tokens[4].c_str());
//	int texID = atoi(tokens[5].c_str());
//
//	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
//	if (tex == NULL)
//	{
//		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
//		return; 
//	}
//
//	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
//}

//void CPlayScene::_ParseSection_ANIMATIONS(string line)
//{
//	vector<string> tokens = split(line);
//
//	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time
//
//	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());
//
//	LPANIMATION ani = new CAnimation();
//
//	int ani_id = atoi(tokens[0].c_str());
//	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
//	{
//		int sprite_id = atoi(tokens[i].c_str());
//		int frame_time = atoi(tokens[i+1].c_str());
//		ani->Add(sprite_id, frame_time);
//	}
//
//	CAnimations::GetInstance()->Add(ani_id, ani);
//}

//void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
//{
//	vector<string> tokens = split(line);
//
//	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id
//
//	int ani_set_id = atoi(tokens[0].c_str());
//
//	LPANIMATION_SET s = new CAnimationSet();
//
//	CAnimations *animations = CAnimations::GetInstance();
//
//	for (int i = 1; i < tokens.size(); i++)
//	{
//		int ani_id = atoi(tokens[i].c_str());
//		
//		LPANIMATION ani = animations->Get(ani_id);
//		s->push_back(ani);
//	}
//
//	CAnimationSets::GetInstance()->Add(ani_set_id, s);
//}


void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x, y);
		player = (CMario*)obj;

		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_KOOPAS: obj = new CKoopas(); break;
	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	//LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	//obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	TiXmlDocument doc(sceneFilePath.c_str());

	if (doc.LoadFile()) {
		TiXmlElement* root = doc.RootElement();

		TiXmlElement* tmxMap = root->FirstChildElement("TmxMap");
		string mapPath = tmxMap->Attribute("path");
		this->map = GameMap::Load(mapPath);


		//test camera
		this->camera = new Camera();
		this->camera->SetPosition(100, 100);
		this->camera->SetSize(500, 500);

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

void CPlayScene::OnKeyDown(int KeyCode)
{
	player->OnKeyDown(KeyCode);

}

void CPlayScene::OnKeyUp(int KeyCode)
{
	player->OnKeyUp(KeyCode);
}


