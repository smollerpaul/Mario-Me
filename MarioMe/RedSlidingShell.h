#pragma once
#include "NormalKoopas.h"

class RedSlidingShell :
    public NormalKoopas
{
public:
	int lastFlip = 0;
	float deathTime = 0;

	RedSlidingShell();
	RedSlidingShell(CKoopas* master);
	virtual void InitAnimations() override;
	virtual void Update(DWORD dt) override;
	virtual void Render();
	virtual void BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult, vector<LPCOLLISIONEVENT> coEvents) override;
	virtual int GetObjectType();
	static const int ObjectType = 73;
};

