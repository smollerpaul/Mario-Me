#include "Koopas.h"
#include "Game.h"
#include "Camera.h"
#include "EnemiesConstants.h"
#include "WingedKoopas.h"

//koopas sliding shell dung question block chua dxxxxxx question block ko chiu change states
CKoopas::CKoopas()
{
	nx = -1;
	objState = new WingedKoopas(this);
	gravity = GRAVITY;
}

CKoopas::CKoopas(NormalKoopas* objectState)
{
	nx = -1;
	objState =new NormalKoopas(this);
	gravity = GRAVITY;
}

void CKoopas::SetObjectState(NormalKoopas* objectState)
{
	this->objState = objectState;
}

void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	float l, t, r, b;
	NormalKoopas* currentState = objState;
	currentState->GetBoundingBox(l,t,r,b);

	left = l;
	top = t;
	right = r;
	bottom = b;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	NormalKoopas* currentState = objState;
	this->dt = dt;
	vy += gravity * dt;

	currentState->Update(dt);

	dx = vx * dt;
	dy = vy * dt;

	CollisionUpdate(dt, coObjects, coEvents, coEventsResult);
	BehaviorUpdate(dt, coEventsResult); 

	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
	
}

void CKoopas::InitAnimations()
{
	/*if (this->animations.size() < 1) {
		this->animations["Move"] = CAnimations::GetInstance()->Get("ani-green-koopa-troopa-move");
	}*/

	NormalKoopas* currentState = objState;
	currentState->InitAnimations();

}

bool CKoopas::CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy)
{
	return false;
}

void CKoopas::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT> coEvents, vector<LPCOLLISIONEVENT>& coEventsResult)
{
	NormalKoopas* currentState = objState;
	currentState->CollisionUpdate(dt, coObjects, coEvents, coEventsResult);
}

void CKoopas::BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult)
{
	NormalKoopas* currentState = objState;
	currentState->BehaviorUpdate(dt, coEventsResult);
}

int CKoopas::GetObjectType()
{
	return ObjectType;
}

void CKoopas::Render()
{
	NormalKoopas* currentState = objState;
	currentState->Render();

	RenderBoundingBox();

}


