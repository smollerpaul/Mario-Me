#include "ScoreFx.h"
#include "Sprites.h"

ScoreFx::ScoreFx(string score, float xPos, float yPos, float aliveTime)
{
	this->score = score;
	aliveTime = 250;
	width = height = 48;

	this->x = xPos;
	this->y = yPos;
	yBegin = yPos;
}

void ScoreFx::Update(DWORD dt)
{
	if (step == 1) {
		y -= flyUpSpeed * dt;
		if (yBegin - y > 144) {
			step = 2;
		}
	}
	else {
		y += fallDownSpeed * dt;

		if (yBegin - y < 60) {
			alive = 0;
		}
	}
}

void ScoreFx::Render()
{
	if (alive == 0)
		return;
	LPSPRITE scoreSprite = CSprites::GetInstance()->Get("spr-points-in-level-" + score);
	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	
	scoreSprite->Draw(x - camera->GetX() + scoreSprite->width / 2, y - camera->GetY() + scoreSprite->height / 2, 1);

}
