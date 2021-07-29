#pragma once
#include "Utils.h"
#include "Sprites.h"

class Text;

class HUD
{
	Text* livesText;
	Text* scoreText;
	Text* timerText;
	Text* coinText;
	Text* worldText;

	int pMeterLevel = 0;

	DWORD time = 0;

	unordered_map<string, CSprite*> pMeterSprite;

public:
	HUD();

	void Update(DWORD dt);

	void Render();
};

