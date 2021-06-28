#include "FireBall.h"
#include "Camera.h"
#include "Game.h"
#include "GameObject.h"
#include "QuestionBlock.h"
#include "Goomba.h"

FireBall::FireBall()
{
}

FireBall::FireBall(CGameObject* fplayer)
{
	this->player = fplayer;
	
	float pl = 0, pt = 0, pr = 0, pb = 0;
	player->GetBoundingBox(pl, pt, pr, pb);

	nx = player->GetDirection();

	//start position fireball
	if (nx > 0) x = pr;
	else 
		x = pl;

	y = pt + FIREBALL_START_Y;

	vy = GRAVITY;
	width = height = FIREBALL_SIZE;
}

void FireBall::InitAnimations()
{
	if (this->animations.size() < 1) {
		this->animations["FireBall"] = CAnimations::GetInstance()->Get("ani-fire-ball");
	}
}

void FireBall::Update(DWORD dt)
{
	vx = FIREBALL_SPEED * nx;
	vy += GRAVITY * dt;

	CGameObject::Update(dt);

	CGameObject::UpdatePosition();
}

void FireBall::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	coEvents.clear();

	if (this)
		CalcPotentialCollisions(coObjects, coEvents);
}

void FireBall::BehaviorUpdate(DWORD dt)
{
	if (coEvents.size() != 0) {
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.1f;
		y += min_ty * dy + ny * 0.1f;

		/*x += min_tx * dx ;
		y += min_ty * dy ;*/

		if (nx != 0) {
			SetAlive(0);
		}
		if (ny != 0) {
			vy = -FIREBALL_BOUNCE_PUSH;
			DebugOut(L"fireball bounce \n");
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
				SetAlive(0);
			}
		}
		break;

		}
	}
}

void FireBall::Render()
{
	InitAnimations();
	CAnimation* ani = this->animations["FireBall"];

	CGameObject::SetFlipOnNormal(nx);

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();

	float l, t, b, r;
	GetBoundingBox(l, t, r, b);

	ani->Render(x - camera->GetX() + (r - l) / 2, y - camera->GetY() + (b - t) / 2, flip);

	RenderBoundingBox();
}

int FireBall::GetObjectType()
{
	return ObjectType;
}
