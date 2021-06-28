#pragma once
#include "SmallMario.h"
#include "Mario.h"
#include "RacoonTail.h"

class RacoonMario :
    public SmallMario
{
public:
	RacoonMario();
	RacoonMario(CMario* masterObj);
	virtual void InitAnimations() override;
	virtual void Update(DWORD dt) override;
	virtual bool CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy) override;

	virtual void MovementUpdate(DWORD dt);
	virtual void JumpUpdate(DWORD dt);
	virtual void AttackUpdate(DWORD dt);

	virtual void BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult, vector<LPCOLLISIONEVENT> coEvents) override;
	virtual void OnKeyUp(int keyCode) override;
	virtual void OnKeyDown(int keyCode) override;
	virtual void Render();
    static const int ObjectType = 1237;
};

