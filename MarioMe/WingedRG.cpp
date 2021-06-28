#include "WingedRG.h"
#include "Mario.h"
#include "FireBall.h"
#include "GameObject.h"
#include "RedGoomba.h"
#include "NormalRG.h"
#include "EnemiesConstants.h"
#include "Game.h"
#include "Camera.h"

WingedRG::WingedRG() {
}

WingedRG::WingedRG(RedGoomba* masterObj)
{
	this->master = masterObj;
	master->SetState(RG_STATE_WALK);
	master->SetDirection(-1);
	master->vx = master->GetDirection() * RG_WALK_SPEED;
}

void WingedRG::InitAnimations()
{
	if (this->animations.size() < 1) {
		this->animations["Walk"] = CAnimations::GetInstance()->Get("ani-red-para-goomba-walk")->Clone();
		this->animations["Fly"] = CAnimations::GetInstance()->Get("ani-red-para-goomba-fly")->Clone();
	}
}

void WingedRG::Update(DWORD dt)
{
	master->vx = master->nx * RG_WALK_SPEED;

	if (isOnGround == 1) {
		if (master->GetState() == RG_STATE_WALK) {
			walkTime += dt;

			if (walkTime >= RG_WALK_TIME) {
				if (master->GetState() != RG_STATE_JUMP) {
					master->SetState(RG_STATE_JUMP);
				}
				walkTime = 0;
			}
		}
	}
	
	if (master->state == RG_STATE_JUMP) {
		isOnGround = 0;
		master->vy = -RG_JUMP_PUSH - dt* GRAVITY;
		jumpHeight += master->vy * dt;

		if (abs(jumpHeight) >= RG_JUMP_HEIGHT) {
			master->vy = -RG_JUMP_PUSH / 2;
			master->SetState(RG_STATE_WALK);
			jumpHeight = 0;
		}
	}

	master->x += master->dx;
	
	//DebugOut(L"RED GOOMBA  vx: %f, x: %f, dx: %f \n", master->vx,master-> x, master->dx);
}

void WingedRG::Render()
{
	InitAnimations();
	LPANIMATION ani = this->animations["Walk"];

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();

	master->SetFlipOnNormal(master->nx);
	int flip = master->flip;

	float l, t, b, r;
	master->GetBoundingBox(l, t, r, b);

	float mx, my;
	master->GetPosition(mx, my);
	if (master->state == RG_STATE_JUMP)
		ani = this->animations["Fly"];
	ani->Render(mx - camera->GetX() + (r - l) / 2, my - camera->GetY() + (b - t) / 2, flip);
}

void WingedRG::BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult, vector<LPCOLLISIONEVENT> coEvents)
{
	if (coEvents.size() != 0) {
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;
		master->FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		master->x += min_tx * master->dx;
		master->y += min_ty * master->dy;

		if (nx != 0) {
			master->nx = -master->nx;
		}
		if (ny != 0) master->vy = 0;

		isOnGround = 1;
	}
	
	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		switch (e->obj->GetObjectType()) {
		case CMario::ObjectType:
		{
			CMario* mario = dynamic_cast<CMario*>(e->obj);
			
			if (e->ny > 0)
			{
				master->SetObjectState(new NormalRG(this->master));
			}
		}
		break;

		case FireBall::ObjectType:
		{
			FireBall* fireball = dynamic_cast<FireBall*>(e->obj);
			if (e->ny != 0 || e->nx != 0)
			{
				if (master->GetState() != RG_STATE_DIE) {
					master->SetState(RG_STATE_DIE);
				}
				master->SetAlive(0);
				DebugOut(L"RG killed by fireball \n");
			}
		}
		break;

		}
	}
}
