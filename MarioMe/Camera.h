#pragma once
class CGameObject;

class Camera
{
protected:
	float x;
	float y;

	float width;
	float height;

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

};