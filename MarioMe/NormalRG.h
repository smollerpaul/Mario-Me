#pragma once
#include "Utils.h"
#include "GameObject.h"

class RedGoomba;
class CGameObject;

class NormalRG
{
private:
	float deathTimer = 0;
	float reliveTimer = 0;

	float startX = 0;
	float startY = 0;
protected:
	RedGoomba* master = NULL;
	unordered_map<string, LPANIMATION> animations;
	
public:
	int isOnGround = 1;

	NormalRG();
	NormalRG(RedGoomba* master);
	virtual void Render();
	virtual void Update(DWORD dt);
	virtual void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects,
		vector<LPCOLLISIONEVENT> coEvents);

	virtual void BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult, vector<LPCOLLISIONEVENT> coEvents);
	virtual void Reset();
	virtual void InitAnimations();
	virtual int GetObjectType();
};

