#include "NormalGoomba.h"
#include "GameObject.h"
#include "Goomba.h"
#include "Utils.h"
#include "Game.h"
#include "Camera.h"

NormalGoomba::NormalGoomba()
{
}

NormalGoomba::NormalGoomba(CGoomba* master)
{
	this->master = master;

	master->SetState(GOOMBA_STATE_WALK);
	master->vx = master->GetDirection() * GOOMBA_WALK_SPEED;
}

void NormalGoomba::Render()
{
	InitAnimations();
	CAnimation* ani = this->animations["Walk"];

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	float l, t, b, r;
	master->GetBoundingBox(l, t, r, b);

	master->SetFlipOnNormal(master->nx);

	if (master->state == GOOMBA_STATE_FLY)
		ani = this->animations["Fly"];

	if (master->state == GOOMBA_STATE_DIE) {
		ani = this->animations["Die"];
		ani->Render(master->x - camera->GetX() + (r - l) / 2, master->y - camera->GetY() + GOOMBA_DIE_Y + (b - t) / 2, master->flip);
	}
	else
		ani->Render(master->x - camera->GetX() + (r - l) / 2, master->y - camera->GetY() + (b - t) / 2, master->flip);

}

void NormalGoomba::Update(DWORD dt)
{
	if (master->state == GOOMBA_STATE_DIE) {
		deathTimer += dt;
		master->vy = 0;

		if (deathTimer >= GOOMBA_DEATH_TIME)
			master->SetAlive(0);
	}
}

void NormalGoomba::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT> coEvents)
{
	master->coEvents.clear();

	if (master->GetState() != GOOMBA_STATE_DIE)
		master->CalcPotentialCollisions(coObjects, master->coEvents);
}

void NormalGoomba::BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult, vector<LPCOLLISIONEVENT> coEvents)
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
				if (master->state != GOOMBA_STATE_DIE) {
					master->SetState(GOOMBA_STATE_DIE);
				}
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

void NormalGoomba::InitAnimations()
{
	
	if (this->animations.size() < 1) {
		this->animations["Walk"] = CAnimations::GetInstance()->Get("ani-goomba-walk");
		this->animations["Die"] = CAnimations::GetInstance()->Get("ani-goomba-die");
		this->animations["Fly"] = CAnimations::GetInstance()->Get("ani-tan-para-goomba-flap");
	}
	
}

int NormalGoomba::GetObjectType()
{
	return master->ObjectType;
}

