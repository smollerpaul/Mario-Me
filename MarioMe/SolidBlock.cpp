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

void SolidBlock::Render()
{
}
