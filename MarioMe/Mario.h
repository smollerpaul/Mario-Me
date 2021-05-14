#pragma once
#include "GameObject.h"
//float MARIO_WALK_SPEED = 0.27f;
//float MARIO_RUN_SPEED = 0.48f;
//
//float MARIO_WALK_ACCELERATION = 0.000376f;
//float MARIO_RUN_ACCELERATION = 0.0003613f;
//float MARIO_WALK_DRAG_FORCE = 0.0014306f;
//float MARIO_RUN_DRAG_FORCE = 0.0016366f;
//float MARIO_CROUCH_DRAG_FORCE = 0.0008766f;
//float MARIO_SKID_ACCELERATION = 0.001104f;
//
//float MARIO_GRAVITY = 0.002f;
//float MARIO_MIN_JUMP_HEIGHT = 80;
//float MARIO_JUMP_HEIGHT = 97;
//float MARIO_PUSH_FORCE = 0.432f;
//
//float MARIO_MIN_HIGH_JUMP_HEIGHT = 97;
//float MARIO_HIGH_JUMP_HEIGHT = 216;
//float MARIO_SUPER_JUMP_HEIGHT = 300;
//float MARIO_SUPER_PUSH_FORCE = 0.632f;
//
//float MAX_FLY_SPEED = 999.0f;



#define MARIO_WALK_SPEED			0.27f
#define MARIO_RUN_SPEED				0.48f

#define MARIO_RUN_ACCELERATION		0.0003613f
#define MARIO_WALK_ACCELERATION		0.000376f
#define MARIO_SKID_ACCELERATION		0.001104f

#define MARIO_MIN_JUMP_HEIGHT		80
#define MARIO_JUMP_HEIGHT			97

#define MARIO_WALK_DRAG_FORCE		0.0014306f
#define MARIO_RUN_DRAG_FORCE		0.0016366f
#define MARIO_CROUCH_DRAG_FORCE 	0.0008766f

#define MARIO_GRAVITY				0.002f

#define MARIO_JUMP_PUSH				0.0432f
#define MARIO_JUMP_DEFLECT_SPEED	0.2f
#define MARIO_DIE_DEFLECT_SPEED		0.5f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALK			100
#define MARIO_STATE_RUN				200
#define MARIO_STATE_RUN_HIGH_SPEED	300

#define MARIO_STATE_DIE				400
#define MARIO_STATE_CROUCH			500

#define MARIO_STATE_JUMP			600
#define MARIO_STATE_JUMP_HIGH		700
#define MARIO_STATE_JUMP_FALL		800





#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2

#define MARIO_BIG_BBOX_WIDTH	45
#define MARIO_BIG_BBOX_HEIGHT	81

#define MARIO_CROUCH_SUBSTRACT	 26

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_UNTOUCHABLE_TIME 5000

class Camera;

class CMario : public CGameObject
{
	int level;
	int untouchable;

	int skid = 0;
	int highSpeed = 0;

	bool isOnGround = true;

	DWORD untouchable_start;

	float start_x = 0;
	float start_y = 0;

	float accelerate_x = 0;
	float accelerate_y = 0;

	int lastKeyDirection = 0;

public: 
	CMario();
	
	virtual void InitAnimations() override;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;

	virtual void MovementUpdate(DWORD dt);

	virtual void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects , 
		vector<LPCOLLISIONEVENT> coEvents, 
		vector<LPCOLLISIONEVENT>& coEventsResult);

	virtual void BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult);

	virtual void Render() override;

	void SetState(int state);

	void SetSkid(int skid);
	int GetSkid();
	
	void SetIsOnGround(bool onGround);
	bool GetIsOnGround();
	
	void SetLevel(int l) { level = l; }

	void StartUntouchable();
	void ResetUntouchable();
	
	void Reset();
	void ResetFlip();

	void OnKeyUp(int keyCode);
	void OnKeyDown(int keyCode);

	
};