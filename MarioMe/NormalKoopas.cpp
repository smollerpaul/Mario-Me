#include "NormalKoopas.h"
#include "Koopas.h"
#include "EnemiesConstants.h"
#include "FireBall.h"
#include "Mario.h"
#include "Shell.h"
#include "Game.h"
#include "Camera.h"

NormalKoopas::NormalKoopas()
{
}
NormalKoopas::NormalKoopas(CKoopas* master)
{
	this->master = master;
	master->SetState(KOOPAS_STATE_WALK);
	master->vx = master->nx * KOOPAS_WALK_SPEED;
}

void NormalKoopas::InitAnimations()
{
	if (this->animations.size() < 1) {
		this->animations["Move"] = CAnimations::GetInstance()->Get("ani-green-koopa-troopa-move")->Clone();
	}
}

void NormalKoopas::Update(DWORD dt)
{
	master->vx = master->nx * KOOPAS_WALK_SPEED;
	//master->x += master->vx * dt;
}

void NormalKoopas::CanGetThrough(CGameObject* gameObjToCollide, float coEventX, float coEventY)
{
	master->CanGetThrough(gameObjToCollide, coEventX, coEventY);
}

void NormalKoopas::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT> coEvents)
{
	master->coEvents.clear();

	if (master->GetState() != KOOPAS_STATE_DIE)
		master->CalcPotentialCollisions(coObjects, master->coEvents);
}

void NormalKoopas::BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult, vector<LPCOLLISIONEVENT> coEvents)
{
	PlayerData* pd = PlayerData::GetInstance();
	PostCollisionUpdate(dt, coEventsResult, coEvents);

	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		switch (e->obj->GetObjectType()) {

		case CMario::ObjectType:
		{
			CMario* mario = dynamic_cast<CMario*>(e->obj);
			if (e->ny > 0)
			{
				master->SetObjectState(new ShelledKoopas(master));
				//DebugOut(L"[KOOPAS CROUCH TO SHELL]\n");

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
				//EffectVault::GetInstance()->AddEffect(new StarWhipTail(master->x, master->y+20));
			}
		}
		break;

		}
	}
}

void NormalKoopas::Render()
{
	InitAnimations();
	LPANIMATION ani = this->animations["Move"];

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();

	master->SetFlipOnNormalEnemy(master->nx);
	int flip = master->flip;

	float l, t, b, r;
	master->GetBoundingBox(l, t, r, b);

	float mx, my;
	master->GetPosition(mx, my);
	//DebugOut(L"NK: Render: mx: %f, my: %f \n", mx, my);

	ani->Render(mx - camera->GetX() + (r - l)/2 , my - camera->GetY() + (b - t)/2, flip);

	master->RenderBoundingBox();
}

int NormalKoopas::GetObjectType()
{
	return ObjectType;
}

void NormalKoopas::PostCollisionUpdate(DWORD dt, vector<LPCOLLISIONEVENT> &coEventsResult, vector<LPCOLLISIONEVENT> &coEvents)
{
	if (master->coEvents.size() == 0) {
		master->UpdatePosition();
	}

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
		if (ny !=0) master->vy = 0;
		if (ny < 0) isOnGround = 1;
	}
}