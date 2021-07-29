#pragma once
#include "Utils.h"
#include "GameObject.h"

#define WHITESPACE	24
#define ALIGN_LEFT 10


class CSprite;
class FontSet;
class Text: public CGameObject
{
	FontSet* font = nullptr;
	string content = "";
	float letterSpacing = 1;
	int textAlign = ALIGN_LEFT;

public:
	Text();
	virtual void Render() override;
	void SetContent(string content);
	void SetFont(FontSet* font);
	void SetLetterSpacing(float spacing);
	void SetAlign(int textAlign);

};

