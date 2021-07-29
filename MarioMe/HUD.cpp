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
	scoreText->SetPosition(180, 53);

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
	coins.insert(coins.begin(), 2 - coins.size(), '0');
	coinText->SetContent(coins);


	string live = to_string(max(data->GetPlayerLives(), 0));
	livesText->SetContent(live);

	string world = to_string(max(1, 0));
	worldText->SetContent(world);

	pMeterLevel = data->GetPowerMeter();
	pMeterLevel = min(7, max(0, pMeterLevel));

	//DebugOut(L"Pmeter: %d\n", pMeterLevel);

	time += dt;

	

}

void HUD::Render()
{
	if (CGame::GetInstance()->GetCurrentScene()->GetSceneId() == "intro")
		return;

	CGame::GetInstance()->GetDirect3DDevice()->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	LPSPRITE panel = CSprites::GetInstance()->Get("spr-hud-0");
	panel->Draw(21 + panel->width / 2, 4 + panel->height / 2, 1);

	timerText->Render();
	scoreText->Render();
	coinText->Render();
	livesText->Render();
	worldText->Render();

	if (pMeterSprite.size() < 1) {
		pMeterSprite["ArrowOn"] = CSprites::GetInstance()->Get("spr-arrow-0");
		pMeterSprite["ArrowOff"] = CSprites::GetInstance()->Get("spr-arrow-1");
		pMeterSprite["PowerOn"] = CSprites::GetInstance()->Get("spr-p-icon-0");
		pMeterSprite["PowerOff"] = CSprites::GetInstance()->Get("spr-p-icon-1");
		pMeterSprite["CardStar"]= CSprites::GetInstance()->Get("spr-star-man-card-0");
	}

	float begin = 180;
	int width = pMeterSprite["ArrowOff"]->width;

	for (int i = 1; i < 7; i++)
	{
		if (i > pMeterLevel) {
			pMeterSprite["ArrowOff"]->Draw(begin + width / 2, 29 + 21 / 2, 1);
		}
		else {
			pMeterSprite["ArrowOn"]->Draw(begin + width / 2, 29 + 21 / 2, 1);
		}
		begin += width;
	}

	width = pMeterSprite["PowerOn"]->width;
	if (pMeterLevel == 7) {
		pMeterSprite["PowerOn"]->Draw(begin + width / 2, 29 + 21 / 2, 1);
	}
	else {
		pMeterSprite["PowerOff"]->Draw(begin + width / 2, 29 + 21 / 2, 1);
	}

	PlayerData* data = PlayerData::GetInstance();
	int cards = data->GetCardType();

	float cardX = 510;
	if (cards > 0 && cards < 3) {

		if (cards == 1) {
			pMeterSprite["CardStar"]->Draw(cardX + 24 / 2, 31 + 29 / 2, 1);
		}
		if (cards == 2) {
			pMeterSprite["CardStar"]->Draw(cardX + 24 / 2, 31 + 29 / 2, 1);
			pMeterSprite["CardStar"]->Draw(cardX + 75 + 24 / 2, 31 + 29 / 2, 1);
		}
	}
}
