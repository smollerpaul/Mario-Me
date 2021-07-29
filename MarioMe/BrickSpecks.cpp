#include "BrickSpecks.h"

BrickSpecks::BrickSpecks(float xPos, float yPos, float vx, float vy, float aliveTime)
{
	Effects::SetPosition(xPos, yPos, aliveTime);
	width = 6;
	height = 6;

	gravity = 0.0015;
	beginY = yPos;
	this->vx = vx;
	this->vy = vy;

	DebugOut(L" vx: %f, vy: %f\n", vx, vy);
}

void BrickSpecks::Update(DWORD dt)
{
	aliveTimerCount += dt;
	if (aliveTimerCount >= aliveTime)
		alive = 0;

	vy += gravity * dt;

	if (abs(y - beginY) >= 500) {
		vy = 0;
	}

	x += vx * dt;
	y += vy * dt;


}

void BrickSpecks::Render()
{
	if (alive == 0)
		return;

	if (animations.size() < 1) {
		this->animations["Default"]=CAnimations::GetInstance()->Get("ani-brick-debris")->Clone();
	}

	LPANIMATION ani = this->animations["Default"];
	Camera* cam = CGame::GetInstance()->GetCurrentScene()->GetCamera();

	ani->Render(x - cam->GetX() + width / 2, y - cam->GetY() + height / 2, 1);
}
