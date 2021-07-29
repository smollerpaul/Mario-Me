#include "Goomba.h"
#include "Game.h"
#include "Camera.h"
#include "Mario.h"
#include "FireBall.h"
#include "EnemiesConstants.h"
#include "NormalGoomba.h"

CGoomba::CGoomba()
{
	SetState(GOOMBA_STATE_WALK);
	width = height= GOOMBA_BBOX_SIZE;
	gravity = ENEMIES_GRAVITY;
	vx = -GOOMBA_WALK_SPEED;
	vy = 0;
	nx = -1;
	objState=new NormalGoomba(this);
}

void CGoomba::InitAnimations()
{
	NormalGoomba* currentState = objState;
	currentState->InitAnimations();
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + width;
	bottom = y + height;

	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;	
}

void CGoomba::Update(DWORD dt)
{
	NormalGoomba* currentState = objState;
	this->dt = dt;
	vy += gravity * dt;
	currentState->Update(dt);

	dx = vx * dt;
	dy = vy * dt;
}

void CGoomba::Render()
{
	NormalGoomba* currentState = objState;
	currentState->Render();
}

bool CGoomba::CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy)
{
	if (obj->GetObjectType() == RedGoomba::ObjectType)
		return true;
	else return false;
}

void CGoomba::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	NormalGoomba* currentState = objState;
	currentState->CollisionUpdate(dt, coObjects,coEvents);
}

void CGoomba::BehaviorUpdate(DWORD dt)
{
	NormalGoomba* currentState = objState;
	currentState->BehaviorUpdate(dt, coEventsResult, coEvents);

}

void CGoomba::SetObjectState(NormalGoomba* objState)
{
	this->objState = objState;
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			vx = 0;
			vy = 0;
			break;
	}
}

int CGoomba::GetObjectType()
{
	return ObjectType;
}
