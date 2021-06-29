#include "MarioDieFx.h"

MarioDieFx::MarioDieFx(float xPos, float yPos, float aliveTime)
{
	Effects::SetPosition(xPos, yPos, aliveTime);
	width = 45;
	height = 45;
	yBegin = yPos;
}

void MarioDieFx::Update(DWORD dt)
{
	if(step == 1) {
		y -= bounceSpeed * dt;
		if (yBegin - y > 180) {
			step = 2;
		}
	}
	else {
	y += fallDownSpeed * dt;
	}
}

void MarioDieFx::Render()
{
	if (this->animations.size() != 1) {
		this->animations["Die"] = CAnimations::GetInstance()->Get("ani-small-mario-die")->Clone();
	}

	LPANIMATION ani = this->animations["Die"];

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	int flip = 1;

	float l, t, r, b;
	GetBoundingBox(l, t, r, b);

	ani->Render(this->x - camera->GetX() + (r - l) / 2, this->y - camera->GetY() + (b - t) / 2, flip);
}
