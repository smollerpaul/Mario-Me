#include "EndSceneText.h"
#include "Text.h"

EndSceneText::EndSceneText(float x, float y, float aliveTime)
{
	Effects::SetPosition(x, y, aliveTime);
	sentence1 = new Text();
	sentence1->SetFont(CGame::GetInstance()->GetFontSet());
	sentence1->SetContent("COURSE CLEAR !");
	sentence1->SetPosition(7857, 814);

	sentence2 = new Text();
	sentence2->SetFont(CGame::GetInstance()->GetFontSet());
	sentence2->SetContent("YOU GOT A CARD");
	sentence2->SetPosition(7828, 888);

}

void EndSceneText::Update(DWORD dt)
{
	timerDelay += dt;
	timerDelayCard += dt;
	if (timerDelay >= 1000) {
		renderSentence2 = 1;
	}

	if (timerDelay >= 1300) {
		renderCard= 1;
	}

	aliveTimerCount += dt;
	if (aliveTimerCount >= aliveTime) {
		alive = 0;
		CGame::GetInstance()->GetCurrentScene()->textShown = 1;
	}
}

void EndSceneText::Render()
{
	if (alive == 0)
		return;

	sentence1->Render();

	if (renderSentence2 == 1) {
		sentence2->Render();
	}

	if (renderCard == 1) {
		LPSPRITE cardSprite = CSprites::GetInstance()->Get("spr-star-man-card-0");
		Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();

		cardSprite->Draw(8210 - camera->GetX() + cardSprite->width / 2, 856 - camera->GetY() + cardSprite->height / 2, 1);
	}
		
}
