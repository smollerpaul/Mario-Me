#include "Camera.h"
#include "Game.h"
#include "GameObject.h"

Camera::Camera()
{
	x = y = 0;
	width = height = 0;
	player = NULL;
}

Camera::Camera(float xCam, float yCam, float w, float h)
{
	x = xCam;
	y = yCam;
	width = w;
	height = h;
	player = NULL;
}

void Camera::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}
void Camera::SetSize(float w, float h)
{
	width = w;
	height = h;
}

float Camera::GetX()
{
	return x;
}
float Camera::GetY()
{
	return y;
}

float Camera::GetWidth()
{
	return this->width;
}
float Camera::GetHeight()
{
	return this->height;
}

void Camera::SetFocusOnPlayer(CGameObject* player)
{
	this->player = player;
}

CGameObject* Camera::GetPlayer()
{
	return player;
}

void Camera::Render()
{
}

void Camera::Update()
{
	if (player == nullptr)
		return;

	float cx, cy;
	CGame* game = CGame::GetInstance();

	player->GetPosition(cx, cy);

	// set limits of the camera position
	if (this->x < 0) {
		this->x = 0;
	}
	if (this->y < 0) {
		this->y = 0;
	}


	x = cx - game->GetScreenWidth() / 2;
	y = cy - game->GetScreenHeight() / 2;

}