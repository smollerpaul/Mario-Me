#pragma once
#include "GameObject.h"
class SolidBlock :  public CGameObject
{
public:
	SolidBlock();
	SolidBlock(float x, float y);

	float GetX();
	float GetY();

	virtual void Render() override;
	virtual int GetObjectType() override;
	static const int ObjectType = 11;
};

