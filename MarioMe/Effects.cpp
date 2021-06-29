#include "Effects.h"

Effects::Effects()
{
}

void Effects::SetPosition(float xPos, float yPos, float aliveTime)
{
	this->x = xPos;
	this->y = yPos;
	this->aliveTime = aliveTime;
}

void Effects::Render()
{
}

void Effects::Update(DWORD dt)
{
}

void Effects::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
