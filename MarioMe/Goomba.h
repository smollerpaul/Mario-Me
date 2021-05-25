#pragma once
#include "GameObject.h"

#define GOOMBA_WALK_SPEED 0.05f;

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 15
#define GOOMBA_BBOX_HEIGHT_DIE 9

#define GOOMBA_STATE_WALK 100
#define GOOMBA_STATE_DIE 200

#define GOOMBA_ANI_WALKING 0
#define GOOMBA_ANI_DIE 1

class Camera;
class Mario;

class CGoomba : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects=NULL) override;
	virtual void Render();

	
	void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects,
		vector<LPCOLLISIONEVENT> coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult);
	void BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult);

public: 	
	CGoomba();
	virtual void SetState(int state);
	virtual void InitAnimations() override;

	virtual int GetObjectType() override;
	static const int ObjectType = 1;
};