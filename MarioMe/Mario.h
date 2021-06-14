#pragma once
#include "GameObject.h"
#include "MarioConstants.h"

class Camera;

//set up state machine

class CMario : public CGameObject
{
	int level;
	int untouchable;

	int skid = 0;

	int highSpeed = 0;
	int isAtMaxRunSpeed = 0;

	bool isOnGround = true;

	DWORD untouchable_start;

	float start_x = 0;
	float start_y = 0;

	float accelerate_x = 0;
	float accelerate_y = 0;

	float friction_x = 0;

	int finalKeyDirection = 0;
	float jumpStartPosition = 0;

	float floatTimer = 0;
	float attackTimer = 0;

	int isAttacking = 0;
	int isAttackingFire = 0;

	int keepIsAttackingAlive = 0;

	float powerMeter = 0;


public: 
	CMario();
	
	virtual void InitAnimations() override;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	virtual bool CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy) override;

	virtual void MovementUpdate(DWORD dt);
	virtual void JumpUpdate(DWORD dt);
	virtual void AttackUpdate(DWORD dt);

	void RunPowerMeter(DWORD dt);

	virtual void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects , 
		vector<LPCOLLISIONEVENT> coEvents, 
		vector<LPCOLLISIONEVENT>& coEventsResult);

	virtual void BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult);

	virtual void Render() override;

	void SetState(int state);

	void SetSkid(int skid);
	int GetSkid();

	void SetFriction(float friction);
	int GetFriction();
	
	void SetIsOnGround(bool onGround);
	bool GetIsOnGround();
	
	void SetLevel(int l);

	void StartUntouchable();
	void ResetUntouchable();

	void Reset();
	void ResetFlip();

	void ResetAttackTimer();
	void ResetFloatTimer();

	void OnKeyUp(int keyCode);
	void OnKeyDown(int keyCode);

	virtual int GetObjectType() override;
	static const int ObjectType = 1000;
};