#pragma once
#include "Utils.h"

class PlayerData
{
	static PlayerData* instance;

	int marioType = 1234; //small
	int score = 0;
	int coins = 0;

	float gameTime = 300000;
	int playerLives = 4;

	int gameTimeIsOver = 0;

public:
	static PlayerData* GetInstance();

	void UpdateGameTime(DWORD dt);
	float GetGameTime();

	int GetMarioType();
	void SetMariotype(int type);

	int GetScore();
	void SetScore( int score);

	int GetCoins();
	void  SetCoins(int coins);

	int GetPlayerLives();
	void SetPlayerLives(int pl);
	void ResetAll();
};
