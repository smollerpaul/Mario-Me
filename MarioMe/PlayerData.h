#pragma once
class PlayerData
{
	static PlayerData* instance;

	int marioType = 2; //1= small, 2= big, 3= racoon, 4=fire
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

