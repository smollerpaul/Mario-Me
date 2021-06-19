#include "NormalRG.h"
#include "GameObject.h"
#include "RedGoomba.h"
#include "Utils.h"
#include "Mario.h"
#include "FireBall.h"
#include "EnemiesConstants.h"
#include "Game.h"
#include "Camera.h"

NormalRG::NormalRG()
{
	DebugOut(L"vo normalRG\n");
}

NormalRG::NormalRG(RedGoomba* masterObj)
{
	this->master = masterObj;

	master->SetState(RG_STATE_WALK);
	master->vx = master->GetDirection() * RG_WALK_SPEED;
}

void NormalRG::InitAnimations()
{
	if (this->animations.size() < 1) {
		this->animations["Walk"] = CAnimations::GetInstance()->Get("ani-red-goomba-walk")->Clone();
		this->animations["Die"] = CAnimations::GetInstance()->Get("ani-red-goomba-die")->Clone();
	}
}

void NormalRG::Update(DWORD dt)
{
	master->vx = master->nx * RG_WALK_SPEED;

	if (master->GetState() == RG_STATE_DIE) {
		deathTimer += dt;

		if (deathTimer >= GOOMBA_DEATH_TIME)
			master->SetAlive(0);
		master->vy = 0;
	}

	master->x += master->dx;
	//DebugOut(L"RED GOOMBA  vx: %f, x: %f, dx: %f \n", master->vx, master->x, master->dx);

}

void NormalRG::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT> coEvents, vector<LPCOLLISIONEVENT>& coEventsResult)
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
		if (ny != 0) master->vy = 0;

		isOnGround = 1;
	}
}

void NormalRG::BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult)
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
				if (master->GetState() != RG_STATE_DIE) {
					master->SetState(RG_STATE_DIE);
				}
			}
		}
		break;

		case FireBall::ObjectType:
		{
			FireBall* fireball = dynamic_cast<FireBall*>(e->obj);
			if (e->ny != 0 || e->nx != 0)
			{
				if (master->GetState() != RG_STATE_DIE) {
					master->SetState(RG_STATE_DIE);
				}
				master->SetAlive(0);
			}
		}
		break;

		}
	}
}

void NormalRG::Render()
{
	InitAnimations();
	LPANIMATION ani= this->animations["Walk"];

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();

	master->SetFlipOnNormal(master->nx);
	int flip = master->flip;

	float l, t, b, r;
	master->GetBoundingBox(l, t, r, b);

	float mx, my;
	master->GetPosition(mx, my);


	if (master->GetState() == RG_STATE_DIE) {
		ani = this->animations["Die"];
		
		ani->Render( mx- camera->GetX() + (r - l) / 2, my - camera->GetY() + GOOMBA_DIE_Y + (b - t) / 2, flip);
	}
	else
		ani->Render(mx - camera->GetX() + (r - l) / 2, my - camera->GetY() + (b - t) / 2, flip);

}

int NormalRG::GetObjectType()
{
	return master->ObjectType;
}