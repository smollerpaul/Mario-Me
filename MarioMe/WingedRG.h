#pragma once
#include "Utils.h"
#include "NormalRG.h"

class RedGoomba;

class WingedRG: public NormalRG
{
protected:
	float walkTime = 0;
	float jumpHeight = 0;
public:
	WingedRG();
	WingedRG(RedGoomba* master);
	virtual void InitAnimations() override;
	virtual void Update(DWORD dt) override;
	virtual void Render();

	virtual void BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult) override;
};

