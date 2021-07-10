#include "VenusFireBall.h"
#include "Camera.h"
#include "Game.h"
#include "Mario.h"

VenusFireBall::VenusFireBall(float ballVx, float ballVy, float x, float y)
{
	width = height = FIREBALL_SIZE;
	gravity = 0;

	this->x = x;
	this->y = y;

	this->vx = ballVx;
	this->vy = ballVy;
}

void VenusFireBall::Update(DWORD dt)
{
	CGameObject::Update(dt);
	CGameObject::UpdatePosition();
}

void VenusFireBall::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	coEvents.clear();

	if (this)
		CalcPotentialCollisions(coObjects, coEvents);
}

void VenusFireBall::BehaviorUpdate(DWORD dt)
{

}

void VenusFireBall::Render()
{
	if (this->animations.size() < 1) {
		this->animations["FireBall"] = CAnimations::GetInstance()->Get("ani-fire-ball");
	}

	CAnimation* ani = this->animations["FireBall"];

	CGameObject::SetFlipOnNormal(nx);

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();

	float l, t, b, r;
	GetBoundingBox(l, t, r, b);

	ani->Render(x - camera->GetX() + (r - l) / 2, y - camera->GetY() + (b - t) / 2, flip);

}

int VenusFireBall::GetObjectType()
{
	return ObjectType;
}
