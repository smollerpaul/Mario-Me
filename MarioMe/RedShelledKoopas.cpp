#include "RedShelledKoopas.h"
#include "Koopas.h"
#include "EnemiesConstants.h"
#include "Game.h"
#include "Camera.h"
#include "FireBall.h"
#include "Mario.h"
#include "Utils.h"
#include "RedNormalKoopas.h"
#include "RedSlidingShell.h"

RedShelledKoopas::RedShelledKoopas()
{
}

RedShelledKoopas::RedShelledKoopas(CKoopas* master)
{
	this->master = master;
	master->vx = 0;
	master->width = master->height = KOOPAS_SHELL_SIZE;
}

void RedShelledKoopas::InitAnimations()
{
	if (this->animations.size() < 1) {
		this->animations["Crouch"] = CAnimations::GetInstance()->Get("ani-red-koopa-troopa-crouch")->Clone();
		this->animations["Respawn"] = CAnimations::GetInstance()->Get("ani-red-koopa-troopa-respawning")->Clone();
	}
}

void RedShelledKoopas::Update(DWORD dt)
{
	Keyboard* keyboard = CGame::GetInstance()->GetKeyboard();

	//update being held
	if (master->holder != nullptr) {
		if (keyboard->IsKeyDown(DIK_A)) {

			//when mario damange -> release
			if (master->holder->untouchable != 0) {
				master->holder->ReleaseRedShell(this);
				master->ReleaseFromHolder(master->holder);
			}
			else {
				float holderX, holderY, hl, ht, hr, hb;
				int holderNx = master->holder->GetNormalX();

				master->holder->GetPosition(holderX, holderY);
				master->holder->GetBoundingBox(hl, ht, hr, hb);

				//set pos
				master->y = ht + 20;
				if (holderNx > 0)
					master->x = hr - 10;
				else master->x = hl - KOOPAS_SHELL_SIZE + 10;

				//set vx vy
				master->gravity = 0;
				master->vy = 0;
				master->vy = master->holder->vx;
			}
		}
		else { 
			//release when no longer pressed A
			master->holder->ReleaseRedShell(this);
			master->ReleaseFromHolder(master->holder);
		}
	}

	else {
		crouchTime += dt;
		if (crouchTime >= KOOPAS_CROUCH_TIME) {
			flicker = 1;
		}

		if (flicker == 1) {
			flickerTime += dt;

			if (flickerTime >= KOOPAS_RESPAWN_TIME)
				master->SetObjectState(new RedNormalKoopas(master));
		}
	}
	
}

void RedShelledKoopas::Render()
{
	InitAnimations();
	LPANIMATION ani = this->animations["Crouch"];

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();

	master->SetFlipOnNormalEnemy(master->nx);
	int flip = master->flip;

	float l, t, b, r;
	master->GetBoundingBox(l, t, r, b);

	float mx, my;
	master->GetPosition(mx, my);

	if (flicker == 1) {
		ani = this->animations["Respawn"];
	}

	ani->Render(mx - camera->GetX() + (r - l) / 2, my - camera->GetY() + (b - t) / 2, flip);
}

void RedShelledKoopas::BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult, vector<LPCOLLISIONEVENT> coEvents)
{
	NormalKoopas::PostCollisionUpdate(dt, coEventsResult, coEvents);
	Keyboard* keyboard = CGame::GetInstance()->GetKeyboard();

	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		switch (e->obj->GetObjectType()) {

		case CMario::ObjectType:
		{
			CMario* mario = dynamic_cast<CMario*>(e->obj);

			if (e->ny > 0)
			{
				master->SetObjectState(new RedSlidingShell(master));
			}
			if (e->nx != 0) {
				if (keyboard->IsKeyDown(DIK_A)) {
					master->holder = mario;
				}
				else {
					//touch left ->kick
					master->SetObjectState(new RedSlidingShell(master));
					//touch right
					if (e->nx > 0)
						master->nx = -1;
				}
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

int RedShelledKoopas::GetObjectType()
{
    return ObjectType;
}
