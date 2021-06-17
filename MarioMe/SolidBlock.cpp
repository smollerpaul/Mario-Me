#include "SolidBlock.h"

SolidBlock::SolidBlock(): CGameObject()
{
	this->dx = 0;
	this->dy = 0;
}

SolidBlock::SolidBlock(float x, float y): CGameObject()
{
	this->x = x;
	this->y = y;
}

float SolidBlock::GetX()
{
	return x;
}

float SolidBlock::GetY()
{
	return y;
}


bool SolidBlock::CanGetThrough(CGameObject* gameObjToCollide, float coEventNx, float coEventNy)
{
	return false;
}

void SolidBlock::Render()
{
}

int SolidBlock::GetObjectType()
{
	return ObjectType;
}
