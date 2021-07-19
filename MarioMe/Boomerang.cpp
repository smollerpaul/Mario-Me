#include "Boomerang.h"
#include "Camera.h"
#include "Game.h"
#include "BoomBro.h"
#include "Mario.h"

Boomerang::Boomerang(BoomBro* master)
{
	width = 45;
	height = 48;
	this->master = master;

	float pl = 0, pt = 0, pr = 0, pb = 0;
	master->GetBoundingBox(pl, pt, pr, pb);
	nx = master->GetDirection();

	gravity = 0;

	if (nx > 0)
		x = pr;
	else
		x = pl;

	y = pt;

	beginX = x;
	beginY = y;

	vx = nx * BOOMERANG_VX;
	vy = -BOOMERANG_VY;

	SetState(BOOMERANG_STATE_SHOOT);
}

void Boomerang::Update(DWORD dt)
{
	if (state==BOOMERANG_STATE_SHOOT) {
		shootTimer += dt;
		vx = nx * BOOMERANG_VX;
		vy = -BOOMERANG_VY;

		if (shootTimer >= 1500) {
			SetState(BOOMERANG_STATE_RETURN);
			shootTimer = 0;
		}
	}

	if (state==BOOMERANG_STATE_RETURN) {
		vy = BOOMERANG_VY;
		vx = -nx * BOOMERANG_VX;
		aliveTime += dt;

		if (aliveTime >= 700 ) {
			SetAlive(0);
		}
	}
	CGameObject::Update(dt);
	
}

void Boomerang::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	coEvents.clear();

	if (this)
		CalcPotentialCollisions(coObjects, coEvents);

}

void Boomerang::BehaviorUpdate(DWORD dt)
{
	if (coEvents.size() == 0)
		CGameObject::UpdatePosition();

	if (coEvents.size() != 0) {
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx;
		y += min_ty * dy;

		if (nx != 0 || ny != 0) {
			if (state == BOOMERANG_STATE_SHOOT)
				SetState(BOOMERANG_STATE_RETURN);
			else SetAlive(0);
		}

	}
}

void Boomerang::Render()
{

    if (animations.size() < 1) {
        animations["Default"] = CAnimations::GetInstance()->Get("ani-boomerang-spin")->Clone();
    }

	CAnimation* ani = this->animations["Default"];

	CGameObject::SetFlipOnNormal(nx);

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();

	float l, t, b, r;
	GetBoundingBox(l, t, r, b);

	ani->Render(x - camera->GetX() + (r - l) / 2, y - camera->GetY() + (b - t) / 2, flip);
	DebugOut(L"okkkkkkk\n");
}

int Boomerang::GetObjectType()
{
	return ObjectType;
}
