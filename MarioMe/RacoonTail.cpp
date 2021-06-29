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
		x = pr;
	else {
		x = pl - width;
	}

	y = pt + FIREBALL_START_Y;
}

void RacoonTail::Update(DWORD dt)
{
	aliveTimer += dt;
	if (aliveTimer >= MARIO_ATTACK_TIME) {
		aliveTimer = 0;
		SetAlive(0);
	}
}

void RacoonTail::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	coEvents.clear();
	if (this)
		CalcPotentialCollisions(coObjects, coEvents);
}

void RacoonTail::BehaviorUpdate(DWORD dt)
{
	if (coEvents.size() != 0) {
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		/*x += min_tx * dx + nx * 0.1f;
		y += min_ty * dy + ny * 0.1f;*/

		if (nx != 0) {
			SetAlive(0);
		}
	}

	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		switch (e->obj->GetObjectType()) {

		case QuestionBlock::ObjectType:
		{
			QuestionBlock* qb = dynamic_cast<QuestionBlock*>(e->obj);

			if (e->ny != 0 || e->nx != 0)
			{
				EffectVault::GetInstance()->AddEffect(new StarWhipTail(x, y));
				SetAlive(0);
			}
		}
		break;

		case CGoomba::ObjectType:
		{
			CGoomba* qb = dynamic_cast<CGoomba*>(e->obj);

			if (e->nx != 0)
			{
				EffectVault::GetInstance()->AddEffect(new StarWhipTail(x, y));
				SetAlive(0);
			}
		}
		break;

		case RedGoomba::ObjectType:
		{
			RedGoomba* qb = dynamic_cast<RedGoomba*>(e->obj);

			if (e->ny != 0 || e->nx != 0)
			{
				EffectVault::GetInstance()->AddEffect(new StarWhipTail(x, y));
				SetAlive(0);
			}
		}
		break;

		case CKoopas::ObjectType:
		{
			CKoopas* qb = dynamic_cast<CKoopas*>(e->obj);

			if (e->ny != 0 || e->nx != 0)
			{
				EffectVault::GetInstance()->AddEffect(new StarWhipTail(x, y));
				SetAlive(0);
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
