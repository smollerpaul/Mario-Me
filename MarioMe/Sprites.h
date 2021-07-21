#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

using namespace std;

class CSprite
{
public: 
	string id = "";				// Sprite ID in the sprite database

	int left = 0;
	int top = 0;
	int width = 0;
	int height = 0;

	D3DXVECTOR3 pivot;

	LPDIRECT3DTEXTURE9 texture = nullptr;

	CSprite(string id, int left, int top, int right, int bottom, int xPivot, int yPivot, LPDIRECT3DTEXTURE9 tex);
	void Draw(float x, float y, int flip, int alpha = 255);
};

typedef CSprite * LPSPRITE;

/*
	Manage sprite database
*/
class CSprites
{
	static CSprites * __instance;

	unordered_map<string, LPSPRITE> sprites;

public:
	void Add(string id, int left, int top, int right, int bottom, int xpivot, int ypivot, LPDIRECT3DTEXTURE9 tex);
	LPSPRITE Get(string id);
	void Clear();
	static CSprites * GetInstance();
};



