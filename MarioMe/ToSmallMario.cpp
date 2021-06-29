#include "ToSmallMario.h"

ToSmallMario::ToSmallMario(float xPos, float yPos, float aliveTime)
{
	Effects::SetPosition(xPos, yPos, aliveTime);
	width = 45;
	height = 81;
}

void ToSmallMario::Render()
{
	if (this->animations.size() != 1) {
		this->animations["Shrink"] = CAnimations::GetInstance()->Get("ani-mario-shrink-down")->Clone();
	}

	LPANIMATION ani = this->animations["Shrink"];

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	int flip = 1;

	float l, t, r, b;
	GetBoundingBox(l, t, r, b);

	ani->Render(this->x - camera->GetX() + (r - l) / 2, this->y - camera->GetY() + (b - t) / 2, flip);
}

void ToSmallMario::Update(DWORD dt)
{
	aliveTimerCount += dt;
	if (aliveTimerCount > aliveTime) {
		alive = 0;
	}
}
