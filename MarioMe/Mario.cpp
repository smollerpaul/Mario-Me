#include "Utils.h"
#include "Mario.h"
#include "Game.h"
#include "Animations.h"
#include "Keyboard.h"
#include "Camera.h"
#include "PlayerData.h"

//enemy mario chạm right side enemy bị pushed -> not gone through
//chinh mario crouch -> small mario doesnt crouch

using namespace std;

CMario::CMario() : CGameObject()
{
	SetSize(MARIO_WIDTH, MARIO_HEIGHT);
	SetObjectState(new SmallMario(this));
	nx = 1;
	flip = 1;
	SetState(MARIO_STATE_IDLE);
	gravity = MARIO_GRAVITY;
}

void CMario::SetObjectState(SmallMario* objectState)
{
	this->objState = objectState;
}

void CMario::InitAnimations()
{
	SmallMario* currentState = objState;
	currentState->InitAnimations();

	DebugOut(L"done init ani MARIO \n");
}

void CMario::Update(DWORD dt)
{
	vy += gravity * dt;

	SmallMario* currentState = objState;
	currentState->Update(dt);

	if (state != MARIO_STATE_DIE && state != MARIO_STATE_REALLY_DIE && transforming != 1) {
		MovementUpdate(dt);
		RunPowerMeter(dt);
		JumpUpdate(dt);
		AttackUpdate(dt);
	}
	//goi rieng

	//call update ->collision -> behavior outside in playscene
	/*CollisionUpdate(dt, coObjects, coEvents, coEventsResult);
	BehaviorUpdate(dt, coEventsResult);*/

}

bool CMario::CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy)
{
	return untouchable;
	switch (obj->GetObjectType()) {
	case CGoomba::ObjectType:
		return coEventNx != 0;
		break;

	case RedGoomba::ObjectType:
		return coEventNx != 0 || coEventNy < 0;
		break;

	case CKoopas::ObjectType:
		return coEventNx != 0 || coEventNy < 0;
		break;
	}
	
}

void CMario::MovementUpdate(DWORD dt)
{
	SmallMario* currentState = objState;
	currentState->MovementUpdate(dt);
}

void CMario::JumpUpdate(DWORD dt)
{
	SmallMario* currentState = objState;
	currentState->JumpUpdate(dt);
}

void CMario::AttackUpdate(DWORD dt)
{
	SmallMario* currentState = objState;
	currentState->AttackUpdate(dt);
}

void CMario::RunPowerMeter(DWORD dt)
{
	//power meter increase as long as onground , mario is at maxspeed
	if (isOnGround == true && isAtMaxRunSpeed == 1 ) {
			powerMeter = min(powerMeter + PM_INCREASE * dt, PM_MAX);
	}
	else { 
		//powerMeter can't be <0
		powerMeter = max(powerMeter - PM_DECREASE * dt, 0.0f);
	}
}

void CMario::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	coEvents.clear();

	if (state != MARIO_STATE_DIE) 
		CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		CGameObject::UpdatePosition(); 
		//including DIE
	}
}

void CMario::BehaviorUpdate(DWORD dt)
{
	SmallMario* currentState = objState;
	currentState->BehaviorUpdate(dt,coEventsResult,coEvents);
}

void CMario::Render()
{
	SmallMario* currentState = objState;
	currentState->Render();

	//RenderBoundingBox();
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MARIO_STATE_IDLE:
		vx = 0;
		break;
	}
}

int CMario::GetObjectType()
{
	return ObjectType;
}

void CMario::OnKeyUp(int keyCode)
{
	SmallMario* currentState = objState;
	currentState->OnKeyUp(keyCode);
}

void CMario::OnKeyDown(int keyCode)
{
	SmallMario* currentState = objState;
	currentState->OnKeyDown(keyCode);

	switch (keyCode) {
	case DIK_1:
		SetObjectState(new SmallMario(this));
		break;
	case DIK_2:
		SetObjectState(new BigMario(this));
		break;
	case DIK_3:
		SetObjectState(new FireMario(this));
		break;
	case DIK_4:
		SetObjectState(new RacoonMario(this));
		break;
	case DIK_E:
		SetPosition(7600,985);
		break;
	}
}

void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetObjectState(new SmallMario(this));
	SetPosition(96, 1151);
	SetSpeed(0, 0);
	ResetUntouchable();
	ResetFlip();
	visible = 1;
}
void CMario::ResetFlip()
{
	if (flip != 1)
		flip = 1;
}
void CMario::ResetAttackTimer()
{
	attackTimer = 0;
	DebugOut(L"Attack timer resetted!\n");
}
void CMario::ResetFloatTimer()
{
	floatTimer = 0;
}

void CMario::StartUntouchable()
{
	DebugOut(L" MARIO start untouchable\n");
	untouchable = 1;
}
void CMario::ResetUntouchable()
{
	DebugOut(L"Reset Untouchable!\n");
	untouchable = 0;
	untouchableTimer = 0;
}

void CMario::SetIsOnGround(bool onGround)
{
	this->isOnGround = onGround;
}
void CMario::SetSkid(int skid)
{
	this->skid = skid;
}
void CMario::SetFriction(float friction)
{
	this->friction_x = friction;
}

