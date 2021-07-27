#include "BrickBreak.h"

BrickBreak::BrickBreak(float xPos, float yPos, float aliveTime)
{
	Effects::SetPosition(xPos, yPos, aliveTime);
}

void BrickBreak::Update(DWORD dt)
{

}

void BrickBreak::Render()
{
	if (this->animations.size() != 1) {
		this->animations["Default"] = CAnimations::GetInstance()->Get("ani-brick-debris")->Clone();
	}

	LPANIMATION ani = this->animations["Default"];

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	int flip = 1;

	float l, t, r, b;
	GetBoundingBox(l, t, r, b);

	ani->Render(this->x - camera->GetX() + (r - l) / 2, this->y - camera->GetY() + (b - t) / 2, flip);
}
