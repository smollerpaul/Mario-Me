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
#include "GreenMushroom.h"
#include "RedMushroom.h"
#include "Leaf.h"
#include "SlidingShell.h"
#include "Shell.h"
#include "WingedKoopas.h"
#include "EndCard.h"
#include "Void.h"
#include "Venus.h"
#include "PSwitch.h"
#include "Piranha.h"
#include "VenusFireBall.h"

#include "FlyingCard.h"
#include "MarioDieFx.h"
#include "ToBigMario.h"
#include "ToSmallMario.h"
#include "MarioTransform.h"
#include "PlayerData.h"

class CMario;
class CGameObject;

class SmallMario
{
protected:
	CMario* master = nullptr;
	unordered_map<string, LPANIMATION> animations;
public:

	int powerUpLeaf = 0;
	int powerUpMushroom = 0;

	SmallMario();
	SmallMario(CMario* masterObj);
	virtual void InitAnimations() ;
	virtual void Update(DWORD dt) ;
	virtual bool CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy) ;

	virtual void MovementUpdate(DWORD dt);
	virtual void JumpUpdate(DWORD dt);
	virtual void AttackUpdate(DWORD dt);
	virtual void RunPowerMeter(DWORD dt);
	virtual void PostCollisionUpdate(DWORD dt, vector<LPCOLLISIONEVENT>& coEventsResult, vector<LPCOLLISIONEVENT>& coEvents);
	virtual void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects,
		vector<LPCOLLISIONEVENT> coEvents);
	virtual void BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult, vector<LPCOLLISIONEVENT> coEvents);

	virtual void Render();

	virtual void OnKeyUp(int keyCode);
	virtual void OnKeyDown(int keyCode);

	virtual int GetObjectType();
	static const int ObjectType = 1234;

};