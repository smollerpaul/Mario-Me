#pragma once
#include"Utils.h"
#include <unordered_map>

class CGameObject;

class Camera
{
protected:
	unordered_map<int, RECT> regions;
	float x;
	float y;

	float width;
	float height;

	int currentRegion = 0;
	CGameObject* player;
	
public:

	Camera();
	Camera(float xCam, float yCam, float w, float h);
	void Update();
	void Render();

	float GetX();
	float GetY();

	float GetWidth();
	float GetHeight();

	void SetPosition(float x, float y);
	void SetSize(float w, float h);

	void SetFocusOnPlayer(CGameObject* player);
	void ReleasePlayer();
	CGameObject* GetPlayer();

	void AddRegion(RECT rect, int regionId);

	void SetCurrentRegion(int regionId);
	RECT GetCurrentRegion(int regionId);
};