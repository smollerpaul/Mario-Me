#pragma once
#include "GameObject.h"


class Camera;
class Mario;

class CGoomba : public CGameObject
{
public:
	float deathTimer = 0;
	int isOnGround = 0;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects=NULL) override;
	virtual void Render();
	virtual bool CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy) override;

	void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects,
		vector<LPCOLLISIONEVENT> coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult);
	void BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult);

	CGoomba();
	virtual void SetState(int state) override;
	virtual void InitAnimations() override;

	virtual int GetObjectType() override;
	static const int ObjectType = 5;
};