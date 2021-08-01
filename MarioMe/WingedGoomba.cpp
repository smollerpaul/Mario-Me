#include "WingedGoomba.h"

WingedGoomba::WingedGoomba(CGoomba* master)
{
	this->master = master;
	master->SetState(GOOMBA_STATE_WALK);
	master->SetDirection(-1);
	master->vx = master->GetDirection() * GOOMBA_WALK_SPEED;
}

void WingedGoomba::Update(DWORD dt)
{
	master->vx = master->nx * GOOMBA_WALK_SPEED;

	if (isOnGround == 1) {
		if (master->GetState() == GOOMBA_STATE_WALK) {
			walkTime += dt;

			if (walkTime >= 500) {
				if (master->GetState() != GOOMBA_STATE_FLY) {
					master->SetState(GOOMBA_STATE_FLY);
				}
				walkTime = 0;
			}
		}
	}

	if (master->state == GOOMBA_STATE_FLY) {
		isOnGround = 0;
		master->vy = -0.4 - dt * MARIO_GRAVITY;
		jumpHeight += master->vy * dt;

		if (abs(jumpHeight) >= 300) {
			master->vy = -RG_JUMP_PUSH / 2;
			master->SetState(GOOMBA_STATE_WALK);
			jumpHeight = 0;
		}
	}
}

void WingedGoomba::Render()
{
	InitAnimations();
	CAnimation* ani = this->animations["Fly"];

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	float l, t, b, r;
	master->GetBoundingBox(l, t, r, b);

	master->SetFlipOnNormal(master->nx);

	ani->Render(master->x - camera->GetX() + (r - l) / 2, master->y - camera->GetY() + (b - t) / 2, master->flip);
}

void WingedGoomba::BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult, vector<LPCOLLISIONEVENT> coEvents)
{
	if (coEvents.size() == 0) {
		master->UpdatePosition();
		isOnGround = 0;
	}

	if (coEvents.size() != 0) {
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;

		master->FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		master->x += min_tx * master->dx;
		master->y += min_ty * master->dy;

		if (nx != 0) master->vx = -master->vx;
		if (ny != 0) {
			master->vy = 0;
			isOnGround = 1;
		}
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
				master->SetObjectState(new NormalGoomba(master));
			}
		}
		break;

		case SlidingShell::ObjectType:
		{
			SlidingShell* ss = dynamic_cast<SlidingShell*>(e->obj);
			if (e->nx != 0)
			{
				if (master->state != GOOMBA_STATE_DIE) {
					master->SetState(GOOMBA_STATE_DIE);
					EffectVault::GetInstance()->AddEffect(new StarWhipTail(master->x, master->y));
				}
			}
		}
		break;

		case RedSlidingShell::ObjectType:
		{
			RedSlidingShell* ss = dynamic_cast<RedSlidingShell*>(e->obj);
			if (e->nx != 0)
			{
				if (master->state != GOOMBA_STATE_DIE) {
					master->SetState(GOOMBA_STATE_DIE);
					EffectVault::GetInstance()->AddEffect(new StarWhipTail(master->x, master->y));
				}
			}
		}
		break;

		case FireBall::ObjectType:
		{
			FireBall* fireball = dynamic_cast<FireBall*>(e->obj);
			if (e->ny != 0 || e->nx != 0)
			{
				if (master->state != GOOMBA_STATE_DIE) {
					master->SetState(GOOMBA_STATE_DIE);
					master->SetAlive(0);

				}
			}
		}
		break;

		case RacoonTail::ObjectType:
		{
			RacoonTail* tail = dynamic_cast<RacoonTail*>(e->obj);
			if (e->ny != 0 || e->nx != 0)
			{
				if (master->state != GOOMBA_STATE_DIE) {
					master->SetState(GOOMBA_STATE_DIE);
					master->SetAlive(0);
				}
			}
		}
		break;

		case CGoomba::ObjectType:
		{
			CGoomba* gb = dynamic_cast<CGoomba*>(e->obj);
			if (e->nx != 0)
			{
				gb->nx = -gb->nx;
			}
		}
		break;

		}
	}
}

int WingedGoomba::GetObjectType()
{
	return master->ObjectType;
}
