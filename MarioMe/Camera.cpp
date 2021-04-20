#include "Camera.h"
#include "Game.h"
#include "GameObject.h"

float Camera::GetX()
{
	return x;
}

float Camera::GetY()
{
	return y;
}

int Camera::GetWidth()
{
	return this->width;
}

int Camera::GetHeight()
{
	return this->height;
}

void Camera::Update()
{
	if (player == nullptr)
		return;

	float cx, cy;
	CGame* game = CGame::GetInstance();

	player->GetPosition(cx, cy);

	x = cx - game->GetScreenWidth() / 2;
	y = cy - game->GetScreenHeight() / 2;

}

void Camera::SetFocusOnPlayer(CGameObject* player)
{
	this->player = player;
}

void Camera::SetPosition(float x, float y)
{
	this->x = x; 
	this->y = y; 
}

void Camera::SetSize(int width, int height)
{
	this->width = width;
	this->height = height;
}
