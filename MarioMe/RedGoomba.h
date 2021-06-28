#pragma once
#include "GameObject.h"
#include "NormalRG.h"
#include "RacoonTail.h"
#include "FireBall.h"
#include "EnemiesConstants.h"


class Camera;
class Mario;

class RedGoomba :
    public CGameObject
{
public:
	
	NormalRG* objState = nullptr;

	RedGoomba();
	virtual void InitAnimations() override;
	void SetObjectState(NormalRG* objectState);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt) override;
	virtual void Render();
	virtual bool CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy) override;

	void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void BehaviorUpdate(DWORD dt);

	virtual int GetObjectType() override;
	static const int ObjectType = 1;
};

