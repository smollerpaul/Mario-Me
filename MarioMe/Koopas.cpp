#include "Koopas.h"
#include "Game.h"
#include "Camera.h"
#include "EnemiesConstants.h"
#include "Mario.h"
#include "WingedKoopas.h"

CKoopas::CKoopas()
{
	width = height = KOOPAS_WIDTH;
	nx = -1;
	objState = new WingedKoopas(this);
	gravity = GRAVITY;
}

CKoopas::CKoopas(NormalKoopas* objectState)
{
	nx = -1;
	gravity = GRAVITY;
	objState = new NormalKoopas(this);
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

void CKoopas::Update(DWORD dt)
{
	NormalKoopas* currentState = objState;
	vy += gravity * dt;

	currentState->Update(dt);

	CGameObject::Update(dt);
}

void CKoopas::InitAnimations()
{
	NormalKoopas* currentState = objState;
	currentState->InitAnimations();
}

bool CKoopas::CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy)
{
	return false;
}

void CKoopas::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects )
{
	NormalKoopas* currentState = objState;
	currentState->CollisionUpdate(dt, coObjects, coEvents);
}

void CKoopas::BehaviorUpdate(DWORD dt)
{
	NormalKoopas* currentState = objState;
	currentState->BehaviorUpdate(dt, coEventsResult, coEvents);
}

int CKoopas::GetObjectType()
{
	NormalKoopas* currentState = objState;
	return currentState->GetObjectType();
}

void CKoopas::Render()
{
	NormalKoopas* currentState = objState;
	currentState->Render();

	RenderBoundingBox();

}


