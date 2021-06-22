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
	SetObjectState(new BigMario(this));
	x = 0;
	y = 0;
	nx = 1;
	flip = 1;
	untouchable = 0;
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

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	DWORD now = GetTickCount64();
	if (now - untouchable_start > MARIO_UNTOUCHABLE_TIME)
		ResetUntouchable();

	vy += gravity * dt;

	SmallMario* currentState = objState;
	currentState->Update(dt);
	DebugOut(L" AFTER UPDATE:  state: %d\n", state);

	if (state != MARIO_STATE_DIE && state != MARIO_STATE_REALLY_DIE && transforming != 1) {
		MovementUpdate(dt);
		RunPowerMeter(dt);
		JumpUpdate(dt);
		AttackUpdate(dt);
	}
	CollisionUpdate(dt, coObjects, coEvents, coEventsResult);
	BehaviorUpdate(dt, coEventsResult);

	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];	

	DebugOut(L" MARIO: vx: %f  vy: %f state: %d\n", vx, vy, state);
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

void CMario::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT> coEvents, vector<LPCOLLISIONEVENT>& coEventsResult)
{
	coEvents.clear();

	//turn off collision when die
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	else {
		CGameObject::UpdatePosition();
		DebugOut(L"im outttttttttttttt\n");
		return;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		CGameObject::UpdatePosition();
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0; //predefined stuff , value doesnt matter

		float rdx = 0;
		float rdy = 0;

		//is get clamp distance , still got those coEvents
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		// if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		// updated position

		x += min_tx * dx + nx * 0.2f;
		y += min_ty * dy + ny * 0.2f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		SetIsOnGround(true);
	}
}

void CMario::BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult)
{
	SmallMario* currentState = objState;
	currentState->BehaviorUpdate(dt,coEventsResult);
}

void CMario::Render()
{
	SmallMario* currentState = objState;
	currentState->Render();

	RenderBoundingBox();
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
	DebugOut(L" start untouchable\n");
	untouchable = 1;
	untouchable_start = GetTickCount64();
}
void CMario::ResetUntouchable()
{
	untouchable = 0;
	untouchable_start = 0;
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

