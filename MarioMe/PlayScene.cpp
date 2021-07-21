#include "PlayScene.h"
#include "Utils.h"
#include "Sprites.h"
#include "Portal.h"
#include "Camera.h"
#include "Map.h"
#include "EffectVault.h"
#include "Text.h"
#include "Game.h"

using namespace std;

CPlayScene::CPlayScene(string id, string filePath):
	CScene(id, filePath)
{

}

void CPlayScene::Update(DWORD dt)
{
	PlayerData* pd = PlayerData::GetInstance();

	if (EffectVault::GetInstance()->GetMarioIsDead() == 1) {
		int pl = pd->GetPlayerLives();
		if (pl > 0)
			pd->SetPlayerLives(pl - 1);
		else pd->ResetAll();

		CGame::GetInstance()->SwitchScene("overworld");
		EffectVault::GetInstance()->SetMarioIsDead(0);

		return;
	}
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
	
	RemoveObject();

	pd->UpdateGameTime(dt);

	//DebugOut(L"[SCORE]: %d  [COINS]: %d  [Time left]: %f \n ", pd->GetScore(), pd->GetCoins(), pd->GetGameTime());
}

void CPlayScene::Render()
{
	CGame::GetInstance()->SetViewport(camera->GetViewPort());

	this->map->Render();
	vector<LPGAMEOBJECT> renderObjects;
	renderObjects.push_back(player);
	renderObjects.push_back(startText);
	
	for (size_t i = 0; i < objects.size(); i++)
	{
		renderObjects.push_back(objects[i]);
	}

	sort(renderObjects.begin(), renderObjects.end(), [](CGameObject*& a, CGameObject*& b) {
		return a->renderOrder < b->renderOrder;
		});

	for (size_t i = 0; i < renderObjects.size(); i++)
	{
		renderObjects[i]->Render();
	}

	EffectVault::GetInstance()->Render();

	//startText->Render();
	/*for (int i = 0; i < coObjects.size(); i++) {
		coObjects[i]->RenderBoundingBox();
	}*/
}

void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	EffectVault::GetInstance()->ClearEffects();
	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", ToLPCWSTR(sceneFilePath));
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
	Camera* camera = new Camera();
	camera->SetFocusOnPlayer(player);
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", ToLPCWSTR(sceneFilePath));

	TiXmlDocument doc(sceneFilePath.c_str());

	if (doc.LoadFile()) {
		TiXmlElement* root = doc.RootElement();

		TiXmlElement* tmxMap = root->FirstChildElement("TmxMap");
		string mapPath = tmxMap->Attribute("path");
		this->map = GameMap::Load(mapPath);

		TiXmlElement* cam = root->FirstChildElement("Camera");
		for (TiXmlElement* region = cam->FirstChildElement("Region"); region != nullptr; region = region->NextSiblingElement("Region")) {
			int regionId; 
			region->QueryIntAttribute("id", &regionId);
			
			float l, t, r, b;
			region->QueryFloatAttribute("left", &l);
			region->QueryFloatAttribute("top", &t);
			region->QueryFloatAttribute("right", &r);
			region->QueryFloatAttribute("bottom", &b);

			RECT rect{};

			rect.left = l;
			rect.top = t;
			rect.right = r;
			rect.bottom = b;
			
			camera->AddRegion(rect, regionId);
		}

		doc.Clear();
	}
	
	camera->SetCurrentRegion(0);
	camera->SetSize(CAM_WIDTH_SIZE, CAM_HEIGHT_SIZE);
	camera->SetViewPort(0, 0, CAM_WIDTH_SIZE, CAM_HEIGHT_SIZE);
	SetCamera(camera);

	PlayerData::GetInstance()->ResetGameTime();

	startText = new Text();
	startText->SetFont(CGame::GetInstance()->GetFontSet());
	startText->SetContent("COURSE CLEAR THANKYOU! ");
	startText->SetPosition(145, 1056);
	
}

int CPlayScene::GetSceneType()
{
	return SceneType;
}



