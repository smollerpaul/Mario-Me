#include "PlayerData.h"

PlayerData* PlayerData::instance = nullptr;

PlayerData* PlayerData::GetInstance()
{
	if (instance == nullptr)
		instance = new PlayerData();
	return instance;
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
