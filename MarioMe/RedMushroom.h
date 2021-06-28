#pragma once
#include "GreenMushroom.h"
class RedMushroom :
    public GreenMushroom
{
public:
	virtual void InitAnimations() override;

	static const int ObjectType = 4001;
};

