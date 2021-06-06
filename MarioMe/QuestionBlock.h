#pragma once
#include "GameObject.h"

#define QB_STATE_ACTIVE		100
#define QB_STATE_BOUNCE		300
#define QB_STATE_FROZEN		200
#define QB_SIZE				45
#define QB_BOUNCE_HEIGHT	10
#define QB_GRAVITY			0.004
#define QB_BOUNCE_SPEED		0.4f

class Camera;

class QuestionBlock :
    public CGameObject
{
public:

	float yStill = 0;
	float dyBounce = 0;
	QuestionBlock();

	virtual void InitAnimations() override;
	virtual void SetPosition(float x, float y) override;
	virtual void Render() override;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	virtual bool CanGetThrough(CGameObject* gameObjToCollide, float coEventNx, float coEventNy) override;

	void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects,
		vector<LPCOLLISIONEVENT> coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult);
	void BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult);

	virtual int GetObjectType() override;
	static const int ObjectType = 22;
};

