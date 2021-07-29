#include "IntroBackground.h"
#include "Text.h"
#include "Camera.h"

IntroBackground::IntroBackground(float aliveTime)
{
	Effects::SetPosition(0, 0, aliveTime);

	Camera* cam = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	player1 = new Text();
	player1->SetFont(CGame::GetInstance()->GetFontSet());
	player1->SetContent("1 PLAYER GAME");
	player1->SetPosition(220 - cam->GetX(), 450 - cam->GetY());

	player2 = new Text();
	player2->SetFont(CGame::GetInstance()->GetFontSet());
	player2->SetContent("2 PLAYER GAME");
	player2->SetPosition(220 - cam->GetX(), 490 - cam->GetY());
	
}

void IntroBackground::Update(DWORD dt)
{
	textCueTimer += dt;
	if (textCueTimer >= 750) {
		textCue = 1;
	}
}

void IntroBackground::Render()
{
	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();

	LPSPRITE treeLeft = CSprites::GetInstance()->Get("spr-tree-left");
	LPSPRITE treeRight = CSprites::GetInstance()->Get("spr-tree-right");
	LPSPRITE curtain = CSprites::GetInstance()->Get("spr-bottom-curtain-0");

	treeLeft->Draw(camera->GetX() + treeLeft->width / 2,  368 - camera->GetX() + treeLeft->height / 2,1);
	treeRight->Draw(568- camera->GetX() + treeRight->width / 2, 272 - camera->GetX() + treeRight->height / 2, 1);
	curtain->Draw( camera->GetX() + curtain->width / 2, camera->GetX() + curtain->height / 2, 1);

	if (textCue == 1) {
		player1->Render();
		player2->Render();
	}
}
