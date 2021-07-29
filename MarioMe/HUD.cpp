#include "HUD.h"
#include "Game.h"
#include "Sprites.h"
#include "PlayerData.h"
#include "Text.h"

HUD::HUD()
{
	livesText = new Text();
	livesText->SetFont(CGame::GetInstance()->GetFontSet());
	livesText->SetPosition(109, 53);

	coinText = new Text();
	coinText->SetFont(CGame::GetInstance()->GetFontSet());
	coinText->SetPosition(421, 29);

	scoreText = new Text();
	scoreText->SetFont(CGame::GetInstance()->GetFontSet());
	scoreText->SetPosition(155, 53);

	timerText = new Text();
	timerText->SetFont(CGame::GetInstance()->GetFontSet());
	timerText->SetPosition(397, 53);

	worldText = new Text();
	worldText->SetFont(CGame::GetInstance()->GetFontSet());
	worldText->SetPosition(136, 29);

}

void HUD::Update(DWORD dt)
{
	PlayerData* data = PlayerData::GetInstance();

	string time = to_string(max((int)(data->GetGameTime() / 1000), 0));
	time.insert(time.begin(), 3 - time.size(), '0');
	timerText->SetContent(time);


	string score = to_string(max(data->GetScore(), 0));
	score.insert(score.begin(), 7 - score.size(), '0');
	scoreText->SetContent(score);


	string coins = to_string(max(data->GetCoins(), 0));
	coinText->SetContent(coins);


	string live = to_string(max(data->GetPlayerLives(), 0));
	livesText->SetContent(live);

	string world = to_string(max(1, 0));
	worldText->SetContent(world);
}

void HUD::Render()
{
	CGame::GetInstance()->GetDirect3DDevice()->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	CSprite* panel = CSprites::GetInstance()->Get("spr-hud-0");
	panel->Draw(21 + panel->width / 2, 4 + panel->height / 2, 1);

	timerText->Render();
	scoreText->Render();
	coinText->Render();
	livesText->Render();
	worldText->Render();
}
