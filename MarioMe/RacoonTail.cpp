#include "RacoonTail.h"
#include"MarioConstants.h"
#include "Camera.h"
#include "Goomba.h"
#include "RedGoomba.h"
#include "StarWhipTail.h"
#include "Mario.h"
#include "QuestionBlock.h"

RacoonTail::RacoonTail()
{
	this -> player = nullptr;
}

RacoonTail::RacoonTail(CGameObject* fplayer)
{
	this->player = fplayer;
	
	float pl = 0, pt = 0, pr = 0, pb = 0;
	player->GetBoundingBox(pl, pt, pr, pb);

	nx = player->GetDirection();
	width = height = TAIL_SIZE;

	if (nx > 0)
		x = player->x + (pb - pt) / 2;
	else x = player->x;

	y = player->y + (pr - pl) / 2 + 20;

	vx = nx * TAIL_SPEED;


}

void RacoonTail::Update(DWORD dt)
{
	vx = nx * TAIL_SPEED;

	aliveTimer += dt;
	if (aliveTimer >= TAIL_ALIVE_TIME) {
		aliveTimer = 0;
		SetAlive(0);
		DebugOut(L"tail gone!\n");

		return;
	}
	CGameObject::Update(dt);
	
	//DebugOut(L"vx tail: %f\n", vx);
}

void RacoonTail::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
}

void RacoonTail::BehaviorUpdate(DWORD dt)
{
	if (coEvents.size() == 0) {
		CGameObject:UpdatePosition();
	}

	if (coEvents.size() != 0) {
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

	/*	x += min_tx * dx ;
		y += min_ty * dy ;*/

		if (nx != 0) {
			//SetAlive(0);
		}

		//DebugOut(L"Tail : %d\n", coEventsResult[0]->obj->GetObjectType());
	}

	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		switch (e->obj->GetObjectType()) {

		case CBrick::ObjectType:
		{
			CBrick* brick= dynamic_cast<CBrick*>(e->obj);

			if (e->nx != 0) {
				//brickbreak
				brick->SetAlive(0);
			}
		}
		break;

		case QuestionBlock::ObjectType:
		{
			QuestionBlock* qb = dynamic_cast<QuestionBlock*>(e->obj);

			if (e->ny != 0 || e->nx != 0)
			{
				EffectVault::GetInstance()->AddEffect(new StarWhipTail(x, y-10));
				}
			}
		
		break;

		case CGoomba::ObjectType:
		{
			CGoomba* qb = dynamic_cast<CGoomba*>(e->obj);

			if (e->nx != 0)
			{
				EffectVault::GetInstance()->AddEffect(new StarWhipTail(qb->x, qb->y));
				SetAlive(0);
			}
		}
		break;

		case RedGoomba::ObjectType:
		{
			RedGoomba* qb = dynamic_cast<RedGoomba*>(e->obj);

			if (e->ny != 0 || e->nx != 0)
			{
				EffectVault::GetInstance()->AddEffect(new StarWhipTail(qb->x, qb->y));
				SetAlive(0);
			}
		}
		break;

		case NormalKoopas::ObjectType:
		{
			NormalKoopas* rg = dynamic_cast<NormalKoopas*>(e->obj);

			if (e->ny != 0 || e->nx != 0)
			{
				SetAlive(0);
				EffectVault::GetInstance()->AddEffect(new StarWhipTail(x, y));
			}
		}
		break;

		case SlidingShell::ObjectType:
		{
			SlidingShell* rg = dynamic_cast<SlidingShell*>(e->obj);

			if (e->ny != 0 || e->nx != 0)
			{
				SetAlive(0);
				EffectVault::GetInstance()->AddEffect(new StarWhipTail(x, y));
			}
		}
		break;

		case WingedKoopas::ObjectType:
		{
			WingedKoopas* rg = dynamic_cast<WingedKoopas*>(e->obj);

			if (e->ny != 0 || e->nx != 0)
			{
				SetAlive(0);
				EffectVault::GetInstance()->AddEffect(new StarWhipTail(x, y));
			}
		}
		break;

		case ShelledKoopas::ObjectType:
		{
			ShelledKoopas* rg = dynamic_cast<ShelledKoopas*>(e->obj);

			if (e->ny != 0 || e->nx != 0)
			{
				SetAlive(0);
				EffectVault::GetInstance()->AddEffect(new StarWhipTail(x, y));
			}
		}
		break;

		}
	}
}

void RacoonTail::Render()
{
	RenderBoundingBox();

}

int RacoonTail::GetObjectType()
{
	return ObjectType;
}
