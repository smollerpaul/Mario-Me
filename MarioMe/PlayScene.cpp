#include "PlayScene.h"
#include "Utils.h"
#include "Sprites.h"
#include "Portal.h"
#include "Camera.h"
#include "Map.h"
#include "EffectVault.h"
#include "Text.h"
#include "Game.h"
#include "Grid.h"
#include "EndSceneText.h"
#include <unordered_set>

using namespace std;

CPlayScene::CPlayScene(string id, string filePath) :
	CScene(id, filePath)
{

}

void CPlayScene::Update(DWORD dt)
{
	PlayerData* pd = PlayerData::GetInstance();

#pragma region EndSceneUpdate
	//end scene text
	if (showTextStart == 1 && textShown==0) {
		EffectVault::GetInstance()->AddEffect(new EndSceneText(this->id,7857, 814));
		showTextStart = 0;
		return;
	}

	if (textShown == 1) {
		CGame::GetInstance()->SwitchScene("overworld");
		return;
	}

	//walk end mario
	if (marioWalkStart == 1) {
		player->SetState(MARIO_STATE_WALK);
		walkOutTimer += dt;

		if (walkOutTimer >= 2000) {
			marioWalkStart = 0;
			showTextStart = 1;
			return;
		}
	}

	//die case
	if (EffectVault::GetInstance()->GetMarioIsDead() == 1) {
		int pl = pd->GetPlayerLives();
		if (pl > 0)
			pd->SetPlayerLives(pl - 1);
		else pd->ResetAll();

		EffectVault::GetInstance()->SetMarioIsDead(0);
		DebugOut(L"Run here GetMarioIsDead\n");
		CGame::GetInstance()->SwitchScene("overworld");
		return;
	}

#pragma endregion

#pragma region Update
	coObjects.clear();

	unordered_set<LPGAMEOBJECT> filteredObjects;

	/*for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}*/
	
	grid->FilterByCamera(camera, objects, filteredObjects);

	coObjects.insert(coObjects.end(), filteredObjects.begin(), filteredObjects.end());
	coObjects.insert(coObjects.end(), objects.begin(), objects.end());
	coObjects.push_back(player);

	player->Update(dt);

	for (size_t i = 0; i < coObjects.size(); i++)
	{
		if (coObjects[i]->GetObjectType() == player->GetObjectType()) continue;
		coObjects[i]->Update(dt);
	}

	player->CollisionUpdate(dt, &coObjects);

	for (size_t i = 0; i < coObjects.size(); i++)
	{
		if (coObjects[i]->GetObjectType() == player->GetObjectType()) continue;
		coObjects[i]->CollisionUpdate(dt, &coObjects);
	}

	player->BehaviorUpdate(dt);

	for (size_t i = 0; i < coObjects.size(); i++)
	{
		if (coObjects[i]->GetObjectType() == player->GetObjectType()) continue;
		coObjects[i]->BehaviorUpdate(dt);
	}

	player->ClearCollision();

	for (size_t i = 0; i < coObjects.size(); i++)
	{
		if (coObjects[i]->GetObjectType() == player->GetObjectType()) continue;
		coObjects[i]->ClearCollision();
	}

	camera->Update();
	EffectVault::GetInstance()->Update(dt);

	RemoveObject();

	pd->UpdateGameTime(dt);
#pragma endregion

	//DebugOut(L"[SCORE]: %d  [COINS]: %d  [Time left]: %f \n ", pd->GetScore(), pd->GetCoins(), pd->GetGameTime());
}

void CPlayScene::Render()
{
	CGame::GetInstance()->GetDirect3DDevice()->Clear(0, NULL, D3DCLEAR_TARGET, BACKGROUND_COLOR, 1.0f, 0);

	this->map->Render();

	vector<LPGAMEOBJECT> renderObjects;
	renderObjects.push_back(player);

	for (size_t i = 0; i < coObjects.size(); i++)
	{
		renderObjects.push_back(coObjects[i]);
	}

	sort(renderObjects.begin(), renderObjects.end(), [](CGameObject*& a, CGameObject*& b) {
		return a->renderOrder < b->renderOrder;
		});

	for (size_t i = 0; i < renderObjects.size(); i++)
	{
		renderObjects[i]->Render();
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

	delete player;

	showTextStart = 0;
	textShown = 0;

	EffectVault::GetInstance()->ClearEffects();
	objects.clear();
	coObjects.clear();
	grid->Clear();
	PlayerData::GetInstance()->ResetGameTime();

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

void CPlayScene::RemoveObject()
{

	coObjects.erase(remove_if(coObjects.begin(), coObjects.end(), [](CGameObject*& obj) {
		return obj->GetAlive() == 0;
	}), coObjects.end());

	CScene::RemoveObject();
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
}

int CPlayScene::GetSceneType()
{
	return SceneType;
}




