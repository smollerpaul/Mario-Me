#pragma once
#include "NormalKoopas.h"
class CKoopas;

class WingedKoopas : public NormalKoopas
{
protected:
	float walkTime = 0;
	float jumpHeight = 0;
	float yGround = 0;
	int isOnGround = 0;
public:
	WingedKoopas();
	WingedKoopas(CKoopas* master);
	virtual void InitAnimations() override;
	virtual void Update(DWORD dt) override;
	virtual void Render();
	virtual void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT> coEvents, vector<LPCOLLISIONEVENT>& coEventsResult) override;
	virtual void BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult) override;
	static const int ObjectType = 33;
};

