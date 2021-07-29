#pragma once
#include "GreenMushroom.h"
class RedMushroom :
    public GreenMushroom
{
public:
	virtual void InitAnimations() override;
	virtual void BehaviorUpdate(DWORD dt);
	virtual int GetObjectType() ;
	static const int ObjectType = 4001;
};

