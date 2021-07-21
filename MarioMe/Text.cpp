#include "Text.h"
#include "Sprites.h"
#include "Font.h"
#include "Camera.h"
#include "Game.h"

Text::Text()
{
	renderOrder = 110;
}

void Text::Render()
{
	if (font == nullptr)
		return;

	float lineWidth = 0;

	//calc max width
	for each (char letter in content)
	{
		if (letter == ' ') {
			lineWidth += WHITESPACE;
			continue;
		}

		lineWidth += letterSpacing;
		lineWidth += font->GetLetter(letter)->width;
	}

	float currentX = 0 ;
	
	currentX = x;
	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();

	//render
	for each (char letter in content)
	{
		if (letter == ' ') {
			currentX += WHITESPACE;
			continue;
		}

		LPSPRITE letterSprite = font->GetLetter(letter);
		
		//not enough space to render
		if (currentX + letterSprite->width >= lineWidth)
			break;
		//render

		letterSprite->Draw(currentX - camera->GetX()+ letterSprite->width/2 , y - camera->GetY() + letterSprite->height / 2, 1);

		//calc the x of next letter to render	
		currentX += letterSprite->width + letterSpacing;
		
	}

	DebugOut(L"currentX: %f\n", currentX);
	DebugOut(L"line width: %f \n", lineWidth);
}

void Text::SetContent(string content)
{
	this->content = content;
	for (char letter : content)
		letter = toupper(letter);
}

void Text::SetFont(FontSet* font)
{
	this->font = font;
}

void Text::SetLetterSpacing(float spacing)
{
	this->letterSpacing = spacing;
}

void Text::SetAlign(int textAlign)
{
	this->textAlign = textAlign;
}


