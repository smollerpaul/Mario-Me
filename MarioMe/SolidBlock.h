#pragma once
#include "GameObject.h"
class SolidBlock :  public CGameObject
{
public:
	SolidBlock();
	SolidBlock(float x, float y);

	virtual void Render() override;
};

