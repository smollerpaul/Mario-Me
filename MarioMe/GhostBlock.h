#pragma once
#include "GameObject.h"


class GhostBlock: public CGameObject
{
public:
	GhostBlock();
	GhostBlock(float width, float height);

	virtual void Render() override;
	virtual int GetObjectType() override;
	static const int ObjectType = 12;
};

