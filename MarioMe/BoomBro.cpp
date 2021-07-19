#include "BoomBro.h"
#include "Game.h"
#include "Camera.h"
#include "Mario.h"
#include "FireBall.h"
#include "EnemiesConstants.h"
#include "PlayScene.h"
#include "Boomerang.h"

BoomBro::BoomBro(CPlayScene* ss)
{
	SetState(BOOM_STATE_WALK);
	nx = -1;
	vx = KOOPAS_WALK_SPEED;
	gravity = ENEMIES_GRAVITY;
	width = BOOM_WIDTH;
	height = BOOM_HEIGHT;
	this->scene = ss;
}

void BoomBro::InitAnimations()
{
	if (animations.size() < 1) {
		this->animations["Walk"] = CAnimations::GetInstance()->Get("ani-boomerang-brother-move")->Clone();
		this->animations["Attack"] = CAnimations::GetInstance()->Get("ani-boomerang-brother-attack")->Clone();
		this->animations["Die"] = CAnimations::GetInstance()->Get("ani-boomerang-brother-death")->Clone();
	}
}

void BoomBro::Update(DWORD dt)
{
	if (state == BOOM_STATE_DIE) {
		dieTimer += dt;
		if (dieTimer >= 1000) {
			SetAlive(0);
			return;
		}
			
	}

	vy += dt * gravity;
	CMario* player= scene->GetPlayer();

	if (state == BOOM_STATE_WALK) {
		walkTime += dt;
		
		if (walkTime >= 2000) {
			walkTime = 0;
			vx = 0;
			SetState(BOOM_STATE_SHOOT);
		}
	}

	if (state == BOOM_STATE_SHOOT) {
			Boomerang* bb = new Boomerang(this);
			CGame::GetInstance()->GetCurrentScene()->AddObject(bb);
			SetState(BOOM_STATE_WALK);
			vx = KOOPAS_WALK_SPEED;
	}

	if (player->x < this->x) {
		nx = -1;
	}
	else if (player->x >= this->x) {
		nx = 1;
	}
	
	CGameObject::Update(dt);
}

void BoomBro::Render()
{
	InitAnimations();
	CAnimation* ani = this->animations["Walk"];

	if(state==BOOM_STATE_DIE)
		ani = this->animations["Die"];

	if (state == BOOM_STATE_SHOOT)
		ani = this->animations["Attack"];

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	float l, t, b, r;
	GetBoundingBox(l, t, r, b);

	SetFlipOnNormalEnemy(nx);

	ani->Render(x - camera->GetX() + (r - l) / 2, y - camera->GetY() + (b - t) / 2, flip);

	//RenderBoundingBox();
}

int BoomBro::GetObjectType()
{
    return ObjectType;
}

void BoomBro::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + width;
	bottom = y + height;

	//if (state == BOOM_STATE_DIE)
	//	bottom = y + 39;

}

void BoomBro::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
}

void BoomBro::BehaviorUpdate(DWORD dt)
{
	if (coEvents.size() == 0)
		CGameObject::UpdatePosition();

	if (coEvents.size() != 0) {
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx;
		y += min_ty * dy;

		if (nx != 0) vx = -vx;
		if (ny != 0) vy = 0;
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
				if (state != BOOM_STATE_DIE) {
					SetState(BOOM_STATE_DIE);
				}
			}
		}
		break;
		
		case FireBall::ObjectType:
		{
			FireBall* fireball = dynamic_cast<FireBall*>(e->obj);
			if (e->ny != 0 || e->nx != 0)
			{
				if (state != BOOM_STATE_DIE) {
					SetState(BOOM_STATE_DIE);
					SetAlive(0);

				}
			}
		}
		break;

		case RacoonTail::ObjectType:
		{
			RacoonTail* tail = dynamic_cast<RacoonTail*>(e->obj);
			if (e->ny != 0 || e->nx != 0)
			{
				if (state != BOOM_STATE_DIE) {
					SetState(BOOM_STATE_DIE);
					SetAlive(0);
				}
			}
		}
		break;

		}
	}
}
