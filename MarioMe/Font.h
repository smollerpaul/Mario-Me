#pragma once
#include "Utils.h"
#include "Sprites.h"

struct FontSprite {
	char character;
	string spriteId;
};

class FontSet
{
	unordered_map<char, LPSPRITE> library;

public:
	void InitFontSet(vector<FontSprite> charList);
	LPSPRITE GetLetter(char character);
	
};

