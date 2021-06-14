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
	NormalRG();
	NormalRG(RedGoomba* master);
	virtual void Render();
	virtual void Update(DWORD dt);
	void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects,
		vector<LPCOLLISIONEVENT> coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult);

	void BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult);

	virtual void InitAnimations();
	virtual int GetObjectType();
};

