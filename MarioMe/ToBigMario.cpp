#include "ToBigMario.h"

ToBigMario::ToBigMario(float xPos, float yPos, float aliveTime)
{
	Effects::SetPosition(xPos,yPos,aliveTime);
	width = 45;
	height = 81;
}

void ToBigMario::Update(DWORD dt)
{
	aliveTimerCount += dt;
	if (aliveTimerCount > aliveTime) {
		alive = 0;
	}
}

void ToBigMario::Render()
{
	if (this->animations.size() != 1) {
		this->animations["Grow"] = CAnimations::GetInstance()->Get("ani-mario-grow-up")->Clone();
	}

	LPANIMATION ani = this->animations["Grow"];

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	int flip = 1;

	float l, t, r, b;
	GetBoundingBox(l, t, r, b);

	ani->Render(this->x - camera->GetX() + (r - l) / 2, this->y - camera->GetY() + (b - t) / 2, flip);
}
