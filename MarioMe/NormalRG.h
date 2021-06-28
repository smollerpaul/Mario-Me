#pragma once
#include "Utils.h"
#include "GameObject.h"

class RedGoomba;
class CGameObject;

class NormalRG
{
private:
	float deathTimer = 0;

protected:
	RedGoomba* master;
	unordered_map<string, LPANIMATION> animations;

public:
	int isOnGround = 0;

	NormalRG();
	NormalRG(RedGoomba* master);
	virtual void Render();
	virtual void Update(DWORD dt);
	virtual void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects,
		vector<LPCOLLISIONEVENT> coEvents);

	virtual void BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult, vector<LPCOLLISIONEVENT> coEvents);

	virtual void InitAnimations();
	virtual int GetObjectType();
};

