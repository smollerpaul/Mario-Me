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

void RedGoomba::Update(DWORD dt)
{
	NormalRG* currentState = objState;
	this->dt = dt;
	vy += gravity*dt;

	currentState->Update(dt);

	dx = vx * dt;
	dy = vy * dt;
}

void RedGoomba::Render()
{
	NormalRG* currentState = objState;
	currentState->Render();
}

bool RedGoomba::CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy)
{
	if (obj->GetObjectType() == CGoomba::ObjectType)
		return true;
}

void RedGoomba::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects )
{
	NormalRG* currentState = objState;
	currentState->CollisionUpdate(dt, coObjects,coEvents);
}

void RedGoomba::BehaviorUpdate(DWORD dt)
{
	NormalRG* currentState = objState;
	currentState->BehaviorUpdate(dt, coEventsResult, coEvents);
}

int RedGoomba::GetObjectType()
{
	return ObjectType;
}
