#pragma once
#include "Utils.h"
#include "GameObject.h"

class CKoopas;
class CGameObject;

class NormalKoopas
{
private:
	float stillTimer = 0;
public:
	int isOnGround = 1;
	CKoopas* master;
	unordered_map<string, LPANIMATION> animations;

	NormalKoopas();
	NormalKoopas(CKoopas* master);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
	virtual void Update(DWORD dt);
	virtual void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT> coEvents);
	virtual void PostCollisionUpdate(DWORD dt, vector<LPCOLLISIONEVENT> &coEventsResult, vector<LPCOLLISIONEVENT> &coEvents);
	virtual void BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult , vector<LPCOLLISIONEVENT> coEvents);

	virtual void InitAnimations();
	virtual int GetObjectType();
	static const int ObjectType = 30;

};

