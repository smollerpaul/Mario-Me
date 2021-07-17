#pragma once
#include "NormalKoopas.h"
class CKoopas;

class RedNormalKoopas: public NormalKoopas
{
public:
	RedNormalKoopas();
	RedNormalKoopas(CKoopas* master);
	virtual void InitAnimations() override;
	virtual void Update(DWORD dt) override;
	virtual void BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult, vector<LPCOLLISIONEVENT> coEvents) override;
	static const int ObjectType = 71;
	virtual int GetObjectType();

};

