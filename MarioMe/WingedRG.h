#pragma once
#include "Utils.h"
#include "NormalRG.h"

class RedGoomba;

class WingedRG: public NormalRG
{
protected:

public:
	WingedRG(RedGoomba* master);
	virtual void InitAnimations() override;
	virtual void Update(DWORD dt) override;
	virtual void Render();

	void BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult);
};

