#include "IntroScene.h"
#include "Utils.h"
#include "Map.h"
#include "Camera.h"
#include "PlayScene.h"
#include "SolidBlock.h"
#include "GameTitle.h"
#include "Mario.h"
#include "Curtain.h"
#include "Text.h"
#include "IntroBackground.h"
#include "ArrowPicker.h"

IntroScene::IntroScene(string id, string filePath) :
	CScene(id, filePath)
{
}

void IntroScene::Load()
{
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
	camera->SetViewPort(0, 0, camWidth, camHeight);
	SetCamera(camera);

	m1 = new CMario();
	m1->SetPosition(10, 500);
	m1->SetDirection(1);
	m1->SetObjectState(new BigMario(m1));

	m2 = new CMario();
	m2->SetPosition(692, 500);
	m2->SetDirection(-1);
	m2->SetObjectState(new BigMario(m2));
}

void IntroScene::Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;

	if (m1 && m2) {
		coObjects.push_back(m1);
		coObjects.push_back(m2);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	ScriptUpdate(dt);

	m1->CollisionUpdate(dt, &coObjects);
	m2->CollisionUpdate(dt, &coObjects);

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->CollisionUpdate(dt, &coObjects);
	}

	m1->BehaviorUpdate(dt);
	m2->BehaviorUpdate(dt);

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->BehaviorUpdate(dt);
	}

	m1->ClearCollision();
	m2->ClearCollision();

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->ClearCollision();
	}

	camera->Update();
	EffectVault::GetInstance()->Update(dt);

}

void IntroScene::Render()
{
	if (doneTitle == 1) {
		CGame::GetInstance()->GetDirect3DDevice()->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(248, 236, 160), 1.0f, 0);
	}
	else {
		CGame::GetInstance()->GetDirect3DDevice()->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	}

	this->map->Render();
	
	vector<LPGAMEOBJECT> renderObjects;

	if (m1 && m2) {
		renderObjects.push_back(m1);
		renderObjects.push_back(m2);
	}

	if (arrow) {
		renderObjects.push_back(arrow);
	}

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
}

void IntroScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	EffectVault::GetInstance()->ClearEffects();

	DebugOut(L"[INFO] Scene %s unloaded! \n", ToLPCWSTR(sceneFilePath));
}

void IntroScene::LoadMapObjects(string objectType, vector<D3DXVECTOR2> cells, float x, float y, float width, float height)
{
	if (objectType.compare("SolidBlock") == 0) {
		SolidBlock* solidObject = new SolidBlock();
		solidObject->SetPosition(x, y);
		solidObject->SetSize(width, height);
		AddObject(solidObject);
	}
}

void IntroScene::ScriptUpdate(DWORD dt)
{
	m1->vy += m1->gravity * dt;
	m2->vy += m2->gravity * dt;

	if (scriptStep == -1) {
		EffectVault::GetInstance()->AddEffect(new Curtain(0, 0));
		scriptStep = 0;
	}
	//hold
	if (scriptStep == 0) {
		stepTimer += dt;
		if (stepTimer >= 2000) {
			scriptStep = 1;
			ResetStepTimer();
		}
	}

	//start running
	if (scriptStep == 1) {
		m1->vx = 0.2;
		m2->vx = -0.2;

		m1->SetState(MARIO_STATE_WALK);
		m2->SetState(MARIO_STATE_WALK);

		if (m1->state != MARIO_STATE_JUMP) {
			if (abs(m1->x - m1XBegin) >= 100) {
				m1->SetState(MARIO_STATE_JUMP);
				m1->isOnGround = 0;
				m1->jumpStartPosition = m1->y;
				m1->vy = -MARIO_FLY_PUSH * 3 - MARIO_GRAVITY * dt;
			}
		}

		m1->JumpUpdate(dt);

		if (m1->y <= 300) {
			m1->SetState(MARIO_STATE_JUMP_FALL);
			m1->vy = MARIO_JUMP_PUSH / 2;
			scriptStep = 2;
			return;
		}
	}
	
	if (scriptStep == 2) {
		m1->vy = MARIO_JUMP_PUSH / 2;
		m1->vx = 0.48;

		m2->vx = -0.2;

		stepTimer += dt;
		if (stepTimer >= 690) {
			scriptStep = 3;
			stepTimer = 0;
		}

	}

	if (scriptStep == 3) {
		m2->vx = 0;
		m2->SetState(MARIO_STATE_CROUCH);
		m2->y -= 1;
		m2->SetSize(MARIO_WIDTH, MARIO_HEIGHT - MARIO_CROUCH_SUBSTRACT);

		m1->vx = 0.6;

		if (m1->state != MARIO_STATE_JUMP) {
			m1->SetState(MARIO_STATE_JUMP);
			m1->isOnGround = 0;
			m1->jumpStartPosition = m1->y;
			m1->vy = -MARIO_FLY_PUSH * 3 - MARIO_GRAVITY * dt;
		}

		m1->JumpUpdate(dt);

		if (m1->y <= 2) {
			m1->SetState(MARIO_STATE_JUMP_FALL);
			m1->vy = MARIO_JUMP_PUSH / 2;
			scriptStep = 4;
			m2->y=470;
			return;
		}
	}

	if (scriptStep == 4) {

		m1->SetState(MARIO_STATE_WALK);
		
		m2->SetSize(MARIO_WIDTH, MARIO_HEIGHT);
		m2->SetState(MARIO_STATE_IDLE);
		m2->SetObjectState(new RacoonMario(m2));

		stepTimer += dt;
		if (stepTimer >= 200) {
			scriptStep = 5;
			stepTimer = 0;
			EffectVault::GetInstance()->AddEffect(new GameTitle(100, -150));
		}
	}

	if (scriptStep == 5) {
		m2->SetState(MARIO_STATE_WALK);
		m2->vx = -0.3;
		
		stepTimer += dt;
		if (stepTimer >= 800) {
			scriptStep = 6;
			stepTimer = 0;
		}
	}

	if (scriptStep == 6) {
		m2->vx = 0.3;
		m2->nx = 1;
		if (doneTitle == 1) {
			EffectVault::GetInstance()->AddEffect(new IntroBackground());
			scriptStep = 7;
		}
	}

	if (scriptStep == 7) {
		stepTimer += dt;
		if (stepTimer >= 1000) {
			scriptStep = 8;
			stepTimer = 0;
			arrow = new ArrowPicker();
		}
	}

	m1->Update(dt);
	m2->Update(dt);
}

void IntroScene::OnKeyDown(int KeyCode)
{
	if (arrow)
		arrow->OnKeyDown(KeyCode);
	else return;
}

void IntroScene::OnKeyUp(int KeyCode)
{
	if (arrow)
		arrow->OnKeyUp(KeyCode);
	else return;
}

void IntroScene::ResetStepTimer()
{
	stepTimer = 0;
}

int IntroScene::GetSceneType()
{
	return SceneType;
}


