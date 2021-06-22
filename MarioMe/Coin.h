#pragma once
#include "GameObject.h"

#define COIN_STATE_ACTIVE   100
#define COINT_STATE_INACTIVE 200
#define COIN_SIZE 48


class Camera;
class Coin :
    public CGameObject
{
public:
	Coin();
	virtual void InitAnimations() override;
	virtual void Render() override;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	virtual bool CanGetThrough(CGameObject* gameObjToCollide, float coEventNx, float coEventNy) override;

	void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects,
		vector<LPCOLLISIONEVENT> coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult);
	void BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult);

	virtual int GetObjectType() override;
	static const int ObjectType = 534;

};

