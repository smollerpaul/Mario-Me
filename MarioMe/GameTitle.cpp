#include "GameTitle.h"
#include "Game.h"
#include "IntroBackground.h"


GameTitle::GameTitle(float xPos, float yPos, float aliveTime)
{
	Effects::SetPosition(xPos, yPos, aliveTime);
}

void GameTitle::Update(DWORD dt)
{
	vy = 0.07;
	vx = 0;
	if (y >= 60) {
		vy = 0;
		CGame::GetInstance()->GetCurrentScene()->doneTitle = 1;
	}
	y += vy * dt;
}

void GameTitle::Render()
{
	if (animations.size() < 1) {
		animations["Title"] = CAnimations::GetInstance()->Get("ani-title")->Clone();
		animations["Version"] = CAnimations::GetInstance()->Get("ani-version")->Clone();
		animations["BottomCurtain"] = CAnimations::GetInstance()->Get("ani-bottom-curtain-")->Clone();
	}

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();

	LPANIMATION ani1 = this->animations["Title"];
	LPSPRITE title = CSprites::GetInstance()->Get("spr-title-0");

	LPANIMATION ani2 = this->animations["Version"];
	LPSPRITE ver = CSprites::GetInstance()->Get("spr-version-3");

	ani1->Render(x - camera->GetX() + title->width / 2, y - camera->GetX() + title->height / 2, 1);
	ani2->Render(x + 230 - camera->GetX() + ver->width / 2, y + 218 - camera->GetX() + ver->height / 2, 1);

}
