#include "GhostBlock.h"

GhostBlock::GhostBlock()
{
	this->dx = 0;
	this->dy = 0;
}

GhostBlock::GhostBlock(float width, float height)
{
	this->x = x;
	this->y = y;
}

void GhostBlock::Render()
{
}

bool GhostBlock::CanGetThrough(CGameObject* gameObjToCollide, float coEventNx, float coEventNy)
{
	// bc it can stand on top of a ghost block
	return coEventNy != -1.0f;
}

int GhostBlock::GetObjectType()
{
	return ObjectType;
}

