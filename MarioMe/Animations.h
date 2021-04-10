#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

#include "Sprites.h"

/*
Sprite animation
*/
class CAnimationFrame
{
	LPSPRITE sprite;
	DWORD time;

public:
	CAnimationFrame(LPSPRITE sprite, int time) { this->sprite = sprite; this->time = time; }
	DWORD GetTime() { return time; }
	LPSPRITE GetSprite() { return sprite; }
};

typedef CAnimationFrame *LPANIMATION_FRAME;

class CAnimation
{
	DWORD lastFrameTime;
	int currentFrame;
	int defaultTime;
	vector<LPANIMATION_FRAME> frames;
public:
	CAnimation(int defaultTime = 100) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	void Add(int spriteId, DWORD time = 0);
	CAnimation* Clone();
	void Render(float x, float y,int flip, int alpha = 255);
};

typedef CAnimation *LPANIMATION;

class CAnimations
{
	static CAnimations * __instance;

	unordered_map<string, LPANIMATION> animations;

public:
	void Add(string id, LPANIMATION ani);
	LPANIMATION Get(string id);
	void Clear();
	void AddAnimation(string aniId);

	static CAnimations * GetInstance();
};

typedef vector<LPANIMATION> CAnimationSet;
typedef CAnimations* LPANIMATIONS;

typedef CAnimationSet* LPANIMATION_SET;

/*
	Manage animation set database
*/
class CAnimationSets
{
	static CAnimationSets * __instance;

	unordered_map<string, LPANIMATION_SET> animation_sets;

public:
	CAnimationSets();
	void Add(string id, LPANIMATION_SET ani);
	LPANIMATION_SET Get(string id);


	static CAnimationSets * GetInstance();
};