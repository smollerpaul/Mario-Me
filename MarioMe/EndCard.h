#pragma once
#include "GameObject.h"

#define CARD_STATE_MUSHROOM 10
#define CARD_STATE_FLOWER	20
#define CARD_STATE_STAR		30
#define CARD_STATE_ACHIEVED 40

class EndCard :
    public CGameObject
{
public:
	float swapTime = 0;
    EndCard();
	virtual void InitAnimations() override;
	virtual void Render() override;
	virtual void Update(DWORD dt) override;
	virtual bool CanGetThrough(CGameObject* gameObjToCollide, float coEventNx, float coEventNy) override;

	virtual void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	virtual void BehaviorUpdate(DWORD dt) override;

	virtual int GetObjectType() override;
	static const int ObjectType = 666;
};

