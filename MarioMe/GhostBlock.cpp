#include "GhostBlock.h"

GhostBlock::GhostBlock()
{
	this->dx = 0;
	this->dy = 0;
}



void GhostBlock::Render()
{
}

bool GhostBlock::CanGetThrough(CGameObject* gameObjToCollide, float coEventNx, float coEventNy)
{
	// bc it can stand on top of a ghost block
	return coEventNy >=0 ;
}

int GhostBlock::GetObjectType()
{
	return ObjectType;
}

