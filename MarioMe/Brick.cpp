#include "Brick.h"
#include "Game.h"
#include "Camera.h"

void CBrick::Render()
{
	LPANIMATION ani = this->animations["Default"];
	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	ani->Render(x- camera->GetX(), y-camera->GetY(), flip);
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}

void CBrick::InitAnimations()
{
}

int CBrick::GetObjectType()
{
	return ObjectType;
}
