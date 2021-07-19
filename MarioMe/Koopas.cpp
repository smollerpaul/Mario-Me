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
	gravity = ENEMIES_GRAVITY;
	objState = new WingedKoopas(this);
}

CKoopas::CKoopas(NormalKoopas* objectState)
{
	width = height = KOOPAS_WIDTH;
	nx = -1;
	gravity = ENEMIES_GRAVITY;
	objState = new NormalKoopas(this);
}

void CKoopas::SetObjectState(NormalKoopas* objectState)
{
	this->objState = objectState;
}

void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = this->x;
	top = this->y;
	right = left + width;
	bottom = top + height;
}

void CKoopas::Update(DWORD dt)
{
	this->dt = dt;
	vy += gravity * dt;

	NormalKoopas* currentState = objState;
	currentState->Update(dt);

	dx = vx * dt;
	dy = vy * dt;
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

void CKoopas::SetHolder(CMario* player)
{
	this->holder = player;
}

void CKoopas::ReleaseFromHolder(CMario* player)
{
	this->holder = nullptr;
	SetObjectState(new RedSlidingShell(this)); //left
	
	if (player->nx > 0) //right
		this->nx = 1;

	this->gravity = ENEMIES_GRAVITY;
}

void CKoopas::Render()
{
	NormalKoopas* currentState = objState;
	currentState->Render();

	//RenderBoundingBox();

}


