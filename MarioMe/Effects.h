#pragma once
#include "GameObject.h"
#include "Game.h"
#include "Utils.h"
#include "Camera.h"

class Effects
{
public:
	
	float x = 0;
	float y = 0;

	float vx = 0;
	float vy = 0;

	float width = 0;
	float height = 0;

	float aliveTime = 0;
	float aliveTimerCount = 0;

	int alive = 1;

	unordered_map<string, LPANIMATION> animations;
	unordered_map<string, LPSPRITE> scoreFx;

	Effects();
	void SetPosition(float xPos, float yPos, float aliveTime);
	virtual void Render();
	virtual void Update(DWORD dt);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

};

