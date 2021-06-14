#pragma once
#include "GameObject.h"
#include "NormalRG.h"



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
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	virtual void Render();
	virtual bool CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy) override;

	void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects,
		vector<LPCOLLISIONEVENT> coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult);
	void BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult);

	virtual int GetObjectType() override;
	static const int ObjectType = 1;
};

