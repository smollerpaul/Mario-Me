#include "FireBall.h"
#include "Camera.h"
#include "Game.h"
#include"Goomba.h"
#include "GameObject.h"

// dung tuong bien mat
//dung goomba chet

FireBall::FireBall(CGameObject* fplayer)
{
	this->player = fplayer;
	
	float pl = 0, pt = 0, pr = 0, pb = 0;
	player->GetBoundingBox(pl, pt, pr, pb);

	nx = player->GetDirection();

	if (nx > 0) {
		x = pr;
	}
	else {
		x = pl;
	}
	y = pt + FIREBALL_START_Y;

	vy = GRAVITY;
	vx = FIREBALL_SPEED * nx;

	dx = vx * dt;

	width = height = FIREBALL_SIZE;
}

void FireBall::InitAnimations()
{
	if (this->animations.size() < 1) {
		this->animations["FireBall"] = CAnimations::GetInstance()->Get("ani-fire-ball");
	}
}

void FireBall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CGameObject::Update(dt, coObjects);

	vy += GRAVITY * dt;

	CGameObject::UpdatePosition();

	CollisionUpdate(dt, coObjects, coEvents, coEventsResult);
	BehaviorUpdate(dt, coEventsResult);

	
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];

	DebugOut(L"[fireball] vx: %f, vy: %f\n", vx, vy);
}

void FireBall::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT> coEvents, vector<LPCOLLISIONEVENT>& coEventsResult)
{
	coEvents.clear();

	if (this) //exists
		CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() != 0) {
		//cuz its a moving object
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;


		if (nx != 0) {
			//vx = -FIREBALL_BOUNCE_PUSH;
			SetAlive(0);
			DebugOut(L"touch wall \n");
		} 
		if (ny != 0) {
			vy = - FIREBALL_BOUNCE_PUSH;
			DebugOut(L"bounce \n");
		} 
	}
}

void FireBall::BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult)
{
	// when fireball hit ground , bounces back
	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		switch (e->obj->GetObjectType()) {
			
		case CGoomba::ObjectType:
		{
			CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

			if (e->ny!=0 || e->nx!=0)
			{
				DebugOut(L"Fireball hit Goomba yay!\n");
			}
		}
		break;
		}
	}
}

void FireBall::Render()
{
	InitAnimations();
	CAnimation* ani = this->animations["FireBall"];

	CGameObject::SetFlipOnNormal(nx);

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();

	float l, t, b, r;
	GetBoundingBox(l, t, r, b);

	ani->Render(x - camera->GetX() + (r - l) / 2, y - camera->GetY() + (b - t) / 2, flip);

	RenderBoundingBox();
}

void FireBall::ResetAll()
{
	
}

int FireBall::GetObjectType()
{
	return ObjectType;
}
