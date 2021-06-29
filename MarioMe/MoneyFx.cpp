#include "MoneyFx.h"

MoneyFx::MoneyFx(float xPos, float yPos, float aliveTime)
{
	aliveTime = 250;
	width = height = 48;

	this->x = xPos;
	this->y = yPos;
	yBegin = yPos;
}

void MoneyFx::Update(DWORD dt)
{
	if (step == 1) {
		y -= flyUpSpeed * dt;
		if (yBegin - y > 144) {
			step = 2;
		}
	}
	else {
		DebugOut(L"go here first\n");
		y += fallDownSpeed * dt;

		if (yBegin - y < 40) {
			alive = 0;
			DebugOut(L"Coin will disappear now\n");
		}
	}
}

void MoneyFx::Render()
{
	if (this->animations.size() != 1) {
		this->animations["Default"] = CAnimations::GetInstance()->Get("ani-coin-obtained")->Clone();
	}

	LPANIMATION ani = this->animations["Default"];

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	int flip = 1;

	float l, t, r, b;
	GetBoundingBox(l, t, r, b);

	ani->Render(this->x - camera->GetX() + (r-l) / 2, this->y - camera->GetY() + (b-t) / 2, flip);
}
