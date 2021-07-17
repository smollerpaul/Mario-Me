#include "RedNormalKoopas.h"
#include "Utils.h"
#include "NormalKoopas.h"
#include "EnemiesConstants.h"
#include "Game.h"
#include "Camera.h"
#include "FireBall.h"
#include "Mario.h"
#include "RedShelledKoopas.h"

RedNormalKoopas::RedNormalKoopas()
{
}

RedNormalKoopas::RedNormalKoopas(CKoopas* master)
{
    this->master = master;
    master->SetState(KOOPAS_STATE_WALK);
    master->vx = master->nx * KOOPAS_WALK_SPEED;
}

void RedNormalKoopas::InitAnimations()
{
    if (this->animations.size() < 1) {
        this->animations["Move"] = CAnimations::GetInstance()->Get("ani-red-koopa-troopa-move")->Clone();
    }
}

void RedNormalKoopas::Update(DWORD dt)
{
    master->vx = master->nx * KOOPAS_WALK_SPEED;
}

void RedNormalKoopas::BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult, vector<LPCOLLISIONEVENT> coEvents)
{
	PlayerData* pd = PlayerData::GetInstance();
	NormalKoopas::PostCollisionUpdate(dt, coEventsResult, coEvents);

	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		switch (e->obj->GetObjectType()) {

		case CMario::ObjectType:
		{
			CMario* mario = dynamic_cast<CMario*>(e->obj);
			if (e->ny > 0)
			{
				master->SetObjectState(new RedShelledKoopas(master));
				
				pd->SetScore(pd->GetScore() + 100);
			}
		}
		break;

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

		case RacoonTail::ObjectType:
		{
			RacoonTail* tail = dynamic_cast<RacoonTail*>(e->obj);
			if (e->ny != 0 || e->nx != 0)
			{
				if (master->GetState() != KOOPAS_STATE_DIE) {
					master->SetState(KOOPAS_STATE_DIE);
				}
				master->SetAlive(0);
				
			}
		}
		break;

		}
	}
}

int RedNormalKoopas::GetObjectType()
{
    return ObjectType;
}


