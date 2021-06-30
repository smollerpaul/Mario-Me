#pragma once
#include "NormalKoopas.h"
class CKoopas;

class WingedKoopas : public NormalKoopas
{	
public:
	float walkTime = 0;
	float jumpHeight = 0;
	float yGround = 0;

	WingedKoopas(CKoopas* master);
	virtual void InitAnimations() override;
	virtual void Update(DWORD dt) override;
	virtual void Render();
	virtual void PostCollisionUpdate(DWORD dt, vector<LPCOLLISIONEVENT> &coEventsResult, vector<LPCOLLISIONEVENT> &coEvents) override;
	virtual void BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult, vector<LPCOLLISIONEVENT> coEvents) override;
	static const int ObjectType = 33;
};

