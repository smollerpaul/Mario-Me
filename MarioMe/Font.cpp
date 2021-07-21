#include "Font.h"

//init font while load
void FontSet::InitFontSet(vector<FontSprite> charList)
{
	for (FontSprite charSprite : charList)
		library[charSprite.character] = CSprites::GetInstance()->Get(charSprite.spriteId);
}

LPSPRITE FontSet::GetLetter(char character)
{
	if (library.find(character) == library.end())
		return library.at('0');

	return library.at(character);
}

