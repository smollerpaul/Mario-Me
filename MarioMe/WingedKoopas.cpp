#include "WingedKoopas.h"
#include "Koopas.h"
#include "NormalKoopas.h"
#include "EnemiesConstants.h"
#include "Game.h"
#include "Camera.h"
#include "FireBall.h"
#include "Mario.h"
#include "Utils.h"

WingedKoopas::WingedKoopas(CKoopas* master)
{
	this->master = master;
	master->SetState(KOOPAS_STATE_WALK);
	master->vx = master->nx * KOOPAS_WALK_SPEED;
}

void WingedKoopas::InitAnimations()
{
	if (this->animations.size() < 1 ) {
		this->animations["Move"] = CAnimations::GetInstance()->Get("ani-green-koopa-paratroopa-fly")->Clone();
	}
}

void WingedKoopas::Update(DWORD dt)
{
	master->vx = master->nx * KOOPAS_WALK_SPEED;

	if (isOnGround == 1) {
		if (master->GetState() == KOOPAS_STATE_WALK) {
			walkTime += dt;

			if (walkTime >= KOOPAS_WALK_TIME) {
				if (master->GetState() != KOOPAS_STATE_JUMP) {
					master->SetState(KOOPAS_STATE_JUMP);
				}
				walkTime = 0;
				yGround = master->y;
			}
		}
	}
	
	if (master->state == KOOPAS_STATE_JUMP) {
		isOnGround = 0;
		master->vy = -RG_JUMP_PUSH - dt * MARIO_GRAVITY;
		jumpHeight = master->y - yGround;

		if (abs(jumpHeight) >= KOOPAS_JUMP_HEIGHT) {
			master->vy = -RG_JUMP_PUSH / 1.5;
			master->SetState(KOOPAS_STATE_WALK);
			jumpHeight = 0;
		}
	}

	master->x += master->dx;
}

void WingedKoopas::Render()
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

	ani->Render(mx - camera->GetX() + abs(r - l)/2 , my - camera->GetY() + abs(b - t)/2, flip);
}

void WingedKoopas::PostCollisionUpdate(DWORD dt, vector<LPCOLLISIONEVENT> &coEventsResult, vector<LPCOLLISIONEVENT> &coEvents)
{
	if (coEvents.size() != 0) {
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;
		master->FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		master->x += min_tx * master->dx + nx * 0.2f;
		master->y += min_ty * master->dy + ny * 0.2f;

		if (nx != 0) {
			master->nx = -master->nx;
		}
		if (ny < 0) {
			master->vy = 0;
		}
		else if (ny > 0) {
			//DebugOut(L"kooopas té đi\n");
			master->vy += master->gravity*dt;
		}

		isOnGround = 1;
	}
}

void WingedKoopas::BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult, vector<LPCOLLISIONEVENT> coEvents)
{
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
				master->SetObjectState(new NormalKoopas(this->master));
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
				EffectVault::GetInstance()->AddEffect(new StarWhipTail(master->x, master->y + 20));
			}
		}
		break;

		}
	}

	//DebugOut(L" WING KOOPS vy: %f, isOnGround : %d, STATE: %d \n", master->vy, isOnGround, master->state);
}

