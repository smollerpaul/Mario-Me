#pragma once
#include "SmallMario.h"
#include "Mario.h"

class FireMario :
    public SmallMario
{
public:
	FireMario();
	FireMario(CMario* masterObj);
	virtual void InitAnimations() override;
	virtual void Update(DWORD dt) override;
	virtual bool CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy) override;

	virtual void MovementUpdate(DWORD dt);
	virtual void JumpUpdate(DWORD dt);
	virtual void AttackUpdate(DWORD dt);

	virtual void BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult) override;

	virtual void Render();
	virtual void OnKeyUp(int keyCode) override;
	virtual void OnKeyDown(int keyCode) override;

    static const int ObjectType = 1236;
};

