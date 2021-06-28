#pragma once
#include "GameObject.h"

#define MUSHROOM_WIDTH			44
#define MUSHROOM_HEIGHT			45

#define MUSHROOM_SPEED			0.15f;
#define MUSHROOM_GRAVITY		0.0024f;
#define MUSHROOM_GROWUP_SPEED	0.0012f;

class GreenMushroom :
    public CGameObject
{
public:
	int scriptStep = 1;
	float xBegin = 0;
	float yBegin = 0;

    GreenMushroom();
	virtual void SetPosition(float x, float y) override;
	virtual void InitAnimations() override;
	virtual void Render() override;
	virtual void Update(DWORD dt) override;
	virtual bool CanGetThrough(CGameObject* gameObjToCollide, float coEventNx, float coEventNy) override;

	void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void BehaviorUpdate(DWORD dt);

	virtual int GetObjectType() override;
	static const int ObjectType = 4000;
};

