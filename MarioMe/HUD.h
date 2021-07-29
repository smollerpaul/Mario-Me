#pragma once
#include "Utils.h"

class Text;

class HUD
{
	Text* livesText;
	Text* scoreText;
	Text* timerText;
	Text* coinText;
	Text* worldText;

public:
	HUD();

	void Update(DWORD dt);

	void Render();
};

