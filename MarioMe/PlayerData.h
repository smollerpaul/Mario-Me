#pragma once
class PlayerData
{
	static PlayerData* instance;


	//chinh mariotype theo small
	int marioType = 2;
	int score = 0;
	int coins = 0;

public:
	static PlayerData* GetInstance();

	int GetMarioType();
	void SetMariotype(int type);

	int GetScore();
	void SetScore( int score);

	int GetCoins();
	void  SetCoins(int coins);
};

