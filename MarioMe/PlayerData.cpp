#include "PlayerData.h"

PlayerData* PlayerData::instance = nullptr;

PlayerData* PlayerData::GetInstance()
{
	if (instance == nullptr)
		instance = new PlayerData();
	return instance;
}

void PlayerData::UpdateGameTime(DWORD dt)
{
	if (gameTimeIsOver != 1)
		gameTime -= dt;

	if (gameTime <= 0)
		gameTimeIsOver = 1;
}

float PlayerData::GetGameTime()
{
	return this->gameTime;
}

void PlayerData::ResetGameTime()
{
	this->gameTime = 300000;
}

int PlayerData::GetMarioType()
{
	return marioType;
}

void PlayerData::SetMariotype(int type)
{
	this->marioType = type;
}

int PlayerData::GetScore()
{
	return score;
}

void PlayerData::SetScore(int score)
{
	this->score = score;
}

int PlayerData::GetCoins()
{
	return coins;
}

void PlayerData::SetCoins(int coins)
{
	this->coins = coins;
}

int PlayerData::GetPlayerLives()
{
	return playerLives;
}

void PlayerData::SetPlayerLives(int pl)
{
	this->playerLives = pl;
}

void PlayerData::SetLevelFinished(string sceneId)
{
	//set finish
}

float PlayerData::GetPowerMeter()
{
	return powerMeter;
}

void PlayerData::SetPowerMeter(float pm)
{
	this->powerMeter = pm;
}

void PlayerData::SetCardType(int ct)
{
	if(ct >=2)
		this->cardType = 2;
	else this->cardType = ct;
}

int PlayerData::GetCardType()
{
	return cardType;
}

void PlayerData::ResetAll()
{
	playerLives = 4;
	coins = 0;
	marioType = 1234;
	gameTime = 300000;
	score = 0;

	DebugOut(L"[PLAYER DATA]  RESETTED ALL\n");
}
