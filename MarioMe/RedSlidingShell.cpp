#include "RedSlidingShell.h"
#include "NormalKoopas.h"
#include "Shell.h"
#include "Koopas.h"
#include "EnemiesConstants.h"
#include "Game.h"
#include "Camera.h"
#include "FireBall.h"
#include "Mario.h"
#include "Koopas.h"
#include "Utils.h"
#include "EnemiesConstants.h"
#include "QuestionBlock.h"
#include "Brick.h"

RedSlidingShell::RedSlidingShell()
{
}

RedSlidingShell::RedSlidingShell(CKoopas* master)
{
	this->master = master;
	master->vx = master->nx * KOOPAS_SHELL_SPEED;
	master->width = master->height = KOOPAS_SHELL_SIZE;
}

void RedSlidingShell::InitAnimations()
{
	if (this->animations.size() < 1) {
		this->animations["Move"] = CAnimations::GetInstance()->Get("ani-red-koopa-troopa-shell-run")->Clone();
		this->animations["Respawn"] = CAnimations::GetInstance()->Get("ani-green-koopa-troopa-respawning")->Clone();
	}
}

void RedSlidingShell::Update(DWORD dt)
{
	master->vx = master->nx * KOOPAS_SHELL_SPEED;

	if (master->state == KOOPAS_STATE_DIE) {
		deathTime += dt;

		if (deathTime >= KOOPAS_DEATH_TIME) {
			master->SetAlive(0);
		}
	}
}

void RedSlidingShell::Render()
{
	InitAnimations();
	LPANIMATION ani = this->animations["Move"];

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();

	master->SetFlipOnNormalEnemy(master->nx);

	int flip = master->flip;

	if (flip == lastFlip && lastFlip != 0) {
		flip *= -1;
	}

	float l, t, b, r;
	master->GetBoundingBox(l, t, r, b);

	float mx, my;
	master->GetPosition(mx, my);

	ani->Render(mx - camera->GetX() + (r - l) / 2, my - camera->GetY() + (b - t) / 2, flip);
	lastFlip = flip;

}

void RedSlidingShell::BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult, vector<LPCOLLISIONEVENT> coEvents)
{
	NormalKoopas::PostCollisionUpdate(dt, coEventsResult, coEvents);

	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		switch (e->obj->GetObjectType()) {

		case FireBall::ObjectType:
		{
			FireBall* fireball = dynamic_cast<FireBall*>(e->obj);
			if (e->ny != 0 || e->nx != 0)
			{
				if (master->GetState() != KOOPAS_STATE_DIE) {
					master->SetState(KOOPAS_STATE_DIE);
				}
				master->SetAlive(0);
			}
		}
		break;

		case CBrick::ObjectType:
		{
			CBrick* qb = dynamic_cast<CBrick*>(e->obj);
			if (e->ny != 0 || e->nx != 0)
			{
				qb->SetAlive(0);
				//effect brick break
			}
		}
		break;

		case RacoonTail::ObjectType:
		{
			RacoonTail* tail = dynamic_cast<RacoonTail*>(e->obj);
			if (e->ny != 0 || e->nx != 0)
			{
				if (master->GetState() != KOOPAS_STATE_DIE) {
					master->SetState(KOOPAS_STATE_DIE);
				}
				master->SetAlive(0);
				EffectVault::GetInstance()->AddEffect(new StarWhipTail(master->x, master->y + 20));
			}
		}
		break;

		case QuestionBlock::ObjectType:
		{
			QuestionBlock* qb = dynamic_cast<QuestionBlock*>(e->obj);

			if (e->nx != 0 || e->ny != 0) {
				EffectVault::GetInstance()->AddEffect(new StarWhipTail(master->x, master->y));
			}
		}
		break;

		}
	}
}

int RedSlidingShell::GetObjectType()
{
	return ObjectType;
}


