#pragma once
#include "Utils.h"
#include "GameObject.h"
#include "RedGoomba.h"
#include "Koopas.h"
#include "RacoonTail.h"
#include "StarWhipTail.h"
#include "Mario.h"
#include "FireBall.h"
#include "EnemiesConstants.h"


class CGoomba;
class CGameObject;

class NormalGoomba
{
private:
	float deathTimer = 0;

	float startX = 0;
	float startY = 0;
protected:
	CGoomba* master = NULL;
	unordered_map<string, LPANIMATION> animations;

public:
	int isOnGround = 1;

	NormalGoomba();
	NormalGoomba(CGoomba* master);
	virtual void Render();
	virtual void Update(DWORD dt);
	virtual void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects,
		vector<LPCOLLISIONEVENT> coEvents);
	virtual void BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult, vector<LPCOLLISIONEVENT> coEvents);
	virtual void InitAnimations();
	virtual int GetObjectType() ;
};

