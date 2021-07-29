#pragma once
#include "GameObject.h"

class Camera;
class Mario;
class NormalGoomba;
class CGoomba : public CGameObject
{
public:
	NormalGoomba* objState = nullptr;
	float deathTimer = 0;
	int isOnGround = 0;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	virtual void Update(DWORD dt) override;
	virtual void Render()override ;
	virtual bool CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy) override;

	void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void BehaviorUpdate(DWORD dt);
	void SetObjectState(NormalGoomba* objState);

	CGoomba();
	virtual void SetState(int state) override;
	virtual void InitAnimations() override;

	virtual int GetObjectType() override;
	static const int ObjectType = 5;
};