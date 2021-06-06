#pragma once
#include "GameObject.h"

#define GOOMBA_WALK_SPEED		0.05f;

#define GOOMBA_BBOX_WIDTH		48
#define GOOMBA_BBOX_HEIGHT		48
#define GOOMBA_BBOX_HEIGHT_DIE	27
#define GOOMBA_DIE_Y			21

#define GOOMBA_STATE_WALK		100
#define GOOMBA_STATE_DIE		200

#define GOOMBA_DEATH_TIME		250

class Camera;
class Mario;

class CGoomba : public CGameObject
{
public:
	float deathTimer = 0;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects=NULL) override;
	virtual void Render();
	
	void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects,
		vector<LPCOLLISIONEVENT> coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult);
	void BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult);

	CGoomba();
	virtual void SetState(int state);
	virtual void InitAnimations() override;

	virtual int GetObjectType() override;
	static const int ObjectType = 1;
};