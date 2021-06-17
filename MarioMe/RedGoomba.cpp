#include "RedGoomba.h"
#include "Camera.h"
#include "Mario.h"
#include "FireBall.h"
#include "Goomba.h"
#include "Game.h"
#include "EnemiesConstants.h"
#include "WingedRG.h"

RedGoomba::RedGoomba()
{
	width = height = RG_BBOX_SIZE;
	nx = -1;
	objState = new WingedRG(this);
	gravity = GRAVITY;
}

void RedGoomba::SetObjectState(NormalRG* objectState)
{
	this->objState = objectState;
}

void RedGoomba::InitAnimations()
{
	NormalRG* currentState = objState;
	currentState->InitAnimations();
}

void RedGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + width;
	bottom = y + height;

	if (state == RG_STATE_DIE)
		bottom = y + RG_BBOX_HEIGHT_DIE;
}

void RedGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	
	NormalRG* currentState = objState;
	this->dt = dt;

	currentState->Update(dt);
	// update dx before xet collision
	dx = vx * dt;
	dy = vy * dt;

	CollisionUpdate(dt, coObjects, coEvents, coEventsResult);
	BehaviorUpdate(dt, coEventsResult);

	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}

void RedGoomba::Render()
{
	NormalRG* currentState = objState;
	currentState->Render();

	RenderBoundingBox();
}

bool RedGoomba::CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy)
{
	if (obj->GetObjectType() == CGoomba::ObjectType)
		return true;
}

void RedGoomba::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT> coEvents, vector<LPCOLLISIONEVENT>& coEventsResult)
{
	NormalRG* currentState = objState;
	currentState->CollisionUpdate(dt, coObjects,coEvents, coEventsResult);
}

void RedGoomba::BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult)
{
	NormalRG* currentState = objState;
	currentState->BehaviorUpdate(dt, coEventsResult);
}

int RedGoomba::GetObjectType()
{
	return ObjectType;
}
