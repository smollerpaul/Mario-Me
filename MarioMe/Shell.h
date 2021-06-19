#pragma once
#include "NormalKoopas.h"
class CKoopas;

class ShelledKoopas: public NormalKoopas
{
protected:
	float crouchTime = 0;
	int flicker = 0;
	float flickerTime = 0;
public:
	ShelledKoopas();
	ShelledKoopas(CKoopas* master);
	virtual void InitAnimations() override;
	virtual void Update(DWORD dt) override;
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
	virtual void BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult) override;
};

