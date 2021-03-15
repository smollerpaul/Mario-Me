#pragma once
class CGameObject;

class Camera
{
protected:
	float x = 0, y = 0;
	int width, height;

	CGameObject* player;

public:
	float GetX();
	float GetY();

	void Update();
	void SetFocusOnPlayer(CGameObject* player);
	void SetPosition(float x, float y);
};

