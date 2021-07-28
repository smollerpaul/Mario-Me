#include "FlyingCard.h"

FlyingCard::FlyingCard( float x, float y, float aliveTime)
{
	Effects::SetPosition(x, y, aliveTime);
	width = height = 50;
}

void FlyingCard::Update(DWORD dt)
{
	y -= flyUpSpeed * dt;

	aliveTimerCount += dt;
	if (aliveTimerCount >= aliveTime) {
		alive = 0;
		CGame::GetInstance()->GetCurrentScene()->marioWalkStart=1;
	}
		
}

void FlyingCard::Render()
{
	if (alive == 0)
		return;

	if (this->animations.size() != 1) {
		this->animations["Default"] = CAnimations::GetInstance()->Get("ani-goal-mushroom")->Clone();
	}

	LPANIMATION ani = this->animations["Default"];

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	int flip = 1;

	float l, t, r, b;
	GetBoundingBox(l, t, r, b);

	ani->Render(this->x - camera->GetX() + (r - l) / 2, this->y - camera->GetY() + (b - t) / 2, flip);

}
