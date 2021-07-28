#include "BrickBreak.h"
#include "BrickSpecks.h"
#include "Utils.h"

BrickBreak::BrickBreak(float xPos, float yPos, float vx, float vy, float aliveTime)
{
	Effects::SetPosition(xPos, yPos, aliveTime);

	b1 = new BrickSpecks(x - 30, y - 30, -vx, -vy);
	b2 = new BrickSpecks(x , y - 30, vx, -vy);
	b3 = new BrickSpecks(x - 30, y + 50, -vx, -vy);
	b4 = new BrickSpecks(x , y + 50, vx, -vy);
	b5 = new BrickSpecks(x, y, 0, -vy);
}

void BrickBreak::Update(DWORD dt)
{
	b1->Update(dt);
	b2->Update(dt);
	b3->Update(dt);
	b4->Update(dt);
	b5->Update(dt);
}

void BrickBreak::Render()
{
	b1->Render();
	b2->Render();
	b3->Render();
	b4->Render();
	b5->Render();

}
