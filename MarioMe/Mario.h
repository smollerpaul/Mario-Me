#pragma once
#include "GameObject.h"
#include "MarioConstants.h"
#include "BigMario.h"
#include "FireMario.h"
#include "RacoonMario.h"
#include "SmallMario.h"

class Camera;

class CMario : public CGameObject
{
public:
	RedShelledKoopas* holdingRedShell = nullptr;
	ShelledKoopas* holdingGreenShell = nullptr;

	int untouchable = 0;
	float untouchableTimer = 0;
	int untouchableStep = 0;

	int skid = 0;
	int highSpeed = 0;
	int isAtMaxRunSpeed = 0;
	float accelerate_x = 0;
	float accelerate_y = 0;

	bool isOnGround = true;
	float friction_x = 0;

	int finalKeyDirection = 0;
	float jumpStartPosition = 0;

	float floatTimer = 0;
	float attackTimer = 0;
	float aliveTimer = 0;
	
	float stayPmMaxTimer = 0;
	int stayPmMax = 0;

	int isAttacking = 0;
	int visible = 1;

	int keepIsAttackingAlive = 0;

	float powerMeter = 0;

	SmallMario* objState = nullptr;
	CMario();
	
	void SetObjectState( SmallMario* objectState);

	virtual void InitAnimations() override;
	virtual void Update(DWORD dt) override;
	virtual bool CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy) override;

	virtual void MovementUpdate(DWORD dt);
	virtual void JumpUpdate(DWORD dt);
	virtual void AttackUpdate(DWORD dt);
	void RunPowerMeter(DWORD dt);

	virtual void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void BehaviorUpdate(DWORD dt);

	virtual void Render() override;
	
	void SetState(int state);
	void SetSkid(int skid);
	void SetFriction(float friction);
	void SetIsOnGround(bool onGround);

	void StartUntouchable();
	void ResetUntouchable();

	void Reset();
	void ResetFlip();

	void ResetAttackTimer();
	void ResetFloatTimer();

	void SetHoldingRedShell(RedShelledKoopas* koop);
	void ReleaseRedShell(RedShelledKoopas* koop);

	void SetHoldingGreenShell(ShelledKoopas* koop);
	void ReleaseGreenShell(ShelledKoopas* koop);

	void OnKeyUp(int keyCode);
	void OnKeyDown(int keyCode);

	virtual int GetObjectType() override;
	static const int ObjectType = 987;
};