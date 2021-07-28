#include "Curtain.h"
#include "Game.h"

Curtain::Curtain(float xPos, float yPos, float aliveTime)
{
	width = 768;
	height = 582;

	Effects::SetPosition(xPos, yPos, aliveTime);
}

void Curtain::Update(DWORD dt)
{
	vy = -0.3;
	vx = 0;
	if (y <= -582) {
		vy = 0;
	}
	y += vy * dt;
}

void Curtain::Render()
{
	if (animations.size() < 1) {
		animations["Default"] = CAnimations::GetInstance()->Get("ani-full-curtain-")->Clone();
	}

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	
	LPANIMATION ani = this->animations["Default"];
	ani->Render(x - camera->GetX() + width / 2, y - camera->GetY() + height / 2, 1);

}
