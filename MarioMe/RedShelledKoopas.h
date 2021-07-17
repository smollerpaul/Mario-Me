#pragma once
#include "NormalKoopas.h"

class CKoopas;
class RedShelledKoopas :
    public NormalKoopas
{
protected:
	float crouchTime = 0;
	int flicker = 0;
	float flickerTime = 0;
public:
	RedShelledKoopas();
	RedShelledKoopas(CKoopas* master);
	virtual void InitAnimations() override;
	virtual void Update(DWORD dt) override;
	virtual void Render();
	virtual void BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult, vector<LPCOLLISIONEVENT> coEvents) override;
	static const int ObjectType = 72;
	virtual int GetObjectType();
};

