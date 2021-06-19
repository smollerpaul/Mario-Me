#pragma once
#include "NormalKoopas.h"
class CKoopas;

class SlidingShell :
    public NormalKoopas
{
public:
	int lastFlip = 0;
	SlidingShell();
	SlidingShell(CKoopas* master);
	virtual void InitAnimations() override;
	virtual void Update(DWORD dt) override;
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
	virtual void BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult) override;
};

