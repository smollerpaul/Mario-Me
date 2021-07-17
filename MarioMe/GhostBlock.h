#pragma once
#include "GameObject.h"


class GhostBlock: public CGameObject
{
public:
	GhostBlock();

	virtual void Render() override;
	virtual bool CanGetThrough(CGameObject* gameObjToCollide, float coEventNx, float coEventNy) override;
	virtual int GetObjectType() override;
	static const int ObjectType = 12;
};

