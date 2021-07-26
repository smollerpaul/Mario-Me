#include "Utils.h"
#include "Mario.h"
#include "Game.h"
#include "Animations.h"
#include "Keyboard.h"
#include "Camera.h"
#include "PlayerData.h"

using namespace std;

CMario::CMario() : CGameObject()
{
	SetSize(MARIO_WIDTH, MARIO_HEIGHT);
	SetObjectState(new SmallMario(this));
	nx = 1;
	flip = 1;
	SetState(MARIO_STATE_IDLE);
	gravity = MARIO_GRAVITY;
	renderOrder = 100;
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

	if (state != MARIO_STATE_DIE) {
		MovementUpdate(dt);
		RunPowerMeter(dt);
		JumpUpdate(dt);
		AttackUpdate(dt);
	}

	DebugOut(L"Mario Vy: %f\n", vy);
	//call update ->collision -> behavior outside in playscene
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

	case NormalKoopas::ObjectType:
		return coEventNx != 0 || coEventNy < 0;
		break;

	case SlidingShell::ObjectType:
		return coEventNx != 0 || coEventNy < 0;
		break;

	case WingedKoopas::ObjectType:
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
	if (stayPmMax == 0) {
		if (isOnGround == true && isAtMaxRunSpeed == 1) {
			powerMeter = min(powerMeter + PM_INCREASE * dt, PM_MAX);
		}
		else {
			//powerMeter can't be <0
			powerMeter = max(powerMeter - PM_DECREASE * dt, 0.0f);
		}
	}
	
	//if S is pressed while PM max -> stay that way for 4s
	if (stayPmMax == 1) {
		powerMeter == PM_MAX;

		stayPmMaxTimer += dt;

		if (stayPmMaxTimer >= 4000) {
			stayPmMax = 0;
			stayPmMaxTimer = 0;
		}
	}
	//DebugOut(L"pm is : %f \n", powerMeter);
}

void CMario::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	coEvents.clear();

	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

}

void CMario::BehaviorUpdate(DWORD dt)
{
	SmallMario* currentState = objState;
	currentState->BehaviorUpdate(dt,coEventsResult,coEvents);

	x = max(0, x);
	float l, t, r, b;
	GetBoundingBox(l,t,r,b);

//limit bottom?
}

void CMario::Render()
{
	
	SmallMario* currentState = objState;
	if (untouchable != 1) {
		currentState->Render();
	}
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
		// test end card
		SetPosition(7600,985);
		break;

	case DIK_6:
		SetPosition(912, 1583);
		break;

	case DIK_7:
		//test pswitch
		SetPosition(6240, 985);
		break;
	case DIK_I: 
	{
		CKoopas* kp = new CKoopas(new NormalKoopas());
		kp->SetObjectState(new RedNormalKoopas(kp));
		kp->SetPosition(this->x, this->y);
		CGame::GetInstance()->GetCurrentScene()->AddObject(kp);
	}
		break;
	case DIK_8:
		//test music note
		SetPosition(2112, 985);
		break;

	case DIK_9:
		//test portal
		SetPosition(6720, 290);
		CGame::GetInstance()->GetCurrentScene()->GetCamera()->SetCurrentRegion(0);
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

void CMario::SetHoldingRedShell(RedShelledKoopas* koop) {
	this->holdingRedShell = koop;
}
void CMario::ReleaseRedShell(RedShelledKoopas* koop) {
	this->holdingRedShell = nullptr;
}

void CMario::SetHoldingGreenShell(ShelledKoopas* koop) {
	this->holdingGreenShell = koop;
}
void CMario::ReleaseGreenShell(ShelledKoopas* koop) {
	this->holdingGreenShell = nullptr;
}
