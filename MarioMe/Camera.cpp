#include "Camera.h"
#include "Game.h"
#include "GameObject.h"

Camera::Camera()
{
	x = y = 0;
	width = height = 0;
	player = NULL;

	rect = new RECT();
}

Camera::Camera(float xCam, float yCam, float w, float h)
{
	x = xCam;
	y = yCam;
	width = w;
	height = h;
	player = NULL;

	rect = new RECT{0,0,0,0};
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

void Camera::ReleasePlayer()
{
	if (this->player)
		this->player = NULL;
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

	x = cx - game->GetScreenWidth() / 2;
	y = cy - game->GetScreenHeight() / 2;

	RECT currentRegBound = GetCurrentRegion(currentRegion);

	// set limits of the camera position to allowed region
	if (currentRegion == 1) {
		y = max(y, currentRegBound.top);
	}
	y = min(y, currentRegBound.bottom - this->height);

	x = max(x, currentRegBound.left);
	x = min(x, currentRegBound.right - this->width);

}

void Camera::AddRegion(RECT rect, int regionId)
{
	this->regions[regionId] = rect;

	//DebugOut(L"[CAMERA REGION] added %ld %ld %ld %ld  %d \n ", rect.left, rect.top, rect.right, rect.bottom, regionId);
}

RECT Camera::GetCurrentRegion(int regionId)
{
	return this->regions[regionId];
}

RECT* Camera::GetViewPort()
{
	return rect;
}

void Camera::SetCurrentRegion(int regionId)
{
	this->currentRegion = regionId;
}

void Camera::SetViewPort(float l, float t, float w, float h)
{
	rect->left = l;
	rect->top = t;
	rect->right = l+w;
	rect->bottom = t+h;
}
