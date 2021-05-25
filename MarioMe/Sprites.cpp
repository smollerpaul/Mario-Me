#include "Sprites.h"
#include "Game.h"
#include "Utils.h"

CSprite::CSprite(string id, int left, int top, int width, int height, int xPivot, int yPivot, LPDIRECT3DTEXTURE9 tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->width = width;
	this->height = height;
	this->texture = tex;
	this->pivot = D3DXVECTOR3((float)xPivot, (float)yPivot, 0);
}

void CSprite::Draw(float x, float y, int flip, int alpha)
{
	CGame * game = CGame::GetInstance();
	game->Draw(x, y, pivot, texture, left, top, this->left + this->width, this->top + this->height, alpha, flip);
}



CSprites* CSprites::__instance = NULL;

CSprites* CSprites::GetInstance()
{
	if (__instance == NULL) __instance = new CSprites();
	return __instance;
}

void CSprites::Add(string id, int left, int top, int width, int height, int xpivot, int ypivot, LPDIRECT3DTEXTURE9 tex)
{
	LPSPRITE s = new CSprite(id, left, top, width, height, xpivot, ypivot, tex);
	sprites[id] = s;

	DebugOut(L"[SPRITE] sprite added: %s, %d, %d, %d, %d, %d, %d \n", ToLPCWSTR(id), left, top, width, height, xpivot, ypivot);
}

LPSPRITE CSprites::Get(string id)
{
	if (sprites.find(id) == sprites.end()) 
		return nullptr;
	return sprites[id];
}

void CSprites::Clear()
{
	for (auto x : sprites)
	{
		LPSPRITE s = x.second;
		delete s;
	}

	sprites.clear();
}



