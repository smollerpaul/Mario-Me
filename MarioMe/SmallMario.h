#pragma once
#include "Utils.h"
#include "GameObject.h"
#include "Game.h"
#include "Camera.h"
#include "Keyboard.h"
#include "MarioConstants.h"
#include "FireBall.h"
#include "RedGoomba.h"
#include "Koopas.h"
#include "Goomba.h"
#include "Portal.h"

class CMario;
class CGameObject;

class SmallMario
{
protected:
	CMario* master = nullptr;
	unordered_map<string, LPANIMATION> animations;
public:
	SmallMario();
	SmallMario(CMario* masterObj);
	virtual void InitAnimations() ;
	virtual void Update(DWORD dt) ;
	virtual bool CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy) ;

	virtual void MovementUpdate(DWORD dt);
	virtual void JumpUpdate(DWORD dt);
	virtual void AttackUpdate(DWORD dt);
	virtual void RunPowerMeter(DWORD dt);

	virtual void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects,
		vector<LPCOLLISIONEVENT> coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult);
	virtual void BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult);

	virtual void Render();

	virtual void OnKeyUp(int keyCode);
	virtual void OnKeyDown(int keyCode);

	virtual int GetObjectType();
	static const int ObjectType = 1234;
};

//set power down stuff