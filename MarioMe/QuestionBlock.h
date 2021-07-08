#pragma once
#include "GameObject.h"

#define QB_STATE_ACTIVE		100
#define QB_STATE_BOUNCE		300
#define QB_STATE_FROZEN		200
#define QB_SIZE				45
#define QB_BOUNCE_HEIGHT	10
#define QB_GRAVITY			0.01
#define QB_BOUNCE_SPEED		0.68f

#define COIN_PRIZE			11
#define GMUSH_PRIZE			12
#define RMUSH_PRIZE			13
#define LEAF_PRIZE			14
#define PSWITCH_PRIZE		15

//states -> tail -> player data

class Camera;

class QuestionBlock :
    public CGameObject
{
public:
	int reward = COIN_PRIZE;
	int hasHiddenItem = 0;
	float yStill = 0;
	float dyBounce = 0;

	QuestionBlock();
	QuestionBlock(int blockReward);

	virtual void InitAnimations() override;
	virtual void SetPosition(float x, float y) override;
	virtual void Render() override;
	virtual void Update(DWORD dt) override;
	virtual bool CanGetThrough(CGameObject* gameObjToCollide, float coEventNx, float coEventNy) override;

	void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void BehaviorUpdate(DWORD dt);

	virtual int GetObjectType() override;
	static const int ObjectType = 22;
};

