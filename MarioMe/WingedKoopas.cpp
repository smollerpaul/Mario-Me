#include "WingedKoopas.h"
#include "Koopas.h"
#include "NormalKoopas.h"
#include "EnemiesConstants.h"
#include "Game.h"
#include "Camera.h"
#include "FireBall.h"
#include "Mario.h"
#include "Utils.h"

WingedKoopas::WingedKoopas()
{
}

WingedKoopas::WingedKoopas(CKoopas* master): NormalKoopas()
{
	this->master = master;
	master->SetState(KOOPAS_STATE_WALK);
	master->vx = master->GetDirection() * KOOPAS_WALK_SPEED;
	master->width = KOOPAS_WIDTH;
	master->height = KOOPAS_HEIGHT;
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
		master->vy = -RG_JUMP_PUSH - dt * GRAVITY;
		jumpHeight = master->y - yGround;

		if (abs(jumpHeight) >= KOOPAS_JUMP_HEIGHT) {
			master->vy = -RG_JUMP_PUSH / 1.5;
			master->SetState(KOOPAS_STATE_WALK);
			jumpHeight = 0;
		}
	}

	master->x += master->dx;
	//DebugOut(L"vy: %f, isOnGround : %d, STATE: %d \n", master->vy, isOnGround, master->state);
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

	ani->Render(mx - camera->GetX() + (r - l)/2 , my - camera->GetY() + (b - t)/2, flip);

}

void WingedKoopas::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT> coEvents, vector<LPCOLLISIONEVENT>& coEventsResult)
{
	coEvents.clear();
	
	if (master->GetState() != RG_STATE_DIE)
		master->CalcPotentialCollisions(coObjects, coEvents);
	
	if (coEvents.size() == 0) {
		master->UpdatePosition();
	}
	else {
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;
		master->FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
	
		if (nx != 0) {
			master->nx = -master->nx;
		}
		if (ny != 0) {
			master->vy = 0;
		}
		isOnGround = 1;
	}
}

void WingedKoopas::BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult)
{
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

		}
	}
}

