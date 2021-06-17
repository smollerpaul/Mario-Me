#include "Goomba.h"
#include "Game.h"
#include "Camera.h"
#include "Mario.h"
#include "FireBall.h"
#include "EnemiesConstants.h"
#include "RedGoomba.h"


CGoomba::CGoomba()
{
	SetState(GOOMBA_STATE_WALK);
	width = height= GOOMBA_BBOX_SIZE;
	gravity = GRAVITY;
	vx = -GOOMBA_WALK_SPEED;
}

void CGoomba::InitAnimations()
{
	if (this->animations.size() < 1) {
		this->animations["Walk"] = CAnimations::GetInstance()->Get("ani-goomba-walk");
		this->animations["Die"] = CAnimations::GetInstance()->Get("ani-goomba-die");
	}
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + width;
	bottom = y + height;

	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;	
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	vy += dt* gravity ;

	if (state == GOOMBA_STATE_DIE) {
		deathTimer += dt;
		vy = 0;

		if (deathTimer >= GOOMBA_DEATH_TIME) 
			SetAlive(0);
	}
	CGameObject::Update(dt, coObjects); 
	CollisionUpdate(dt, coObjects, coEvents, coEventsResult);
	BehaviorUpdate(dt, coEventsResult);

	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];

	DebugOut(L" GOOMBA:  vy: %f X: %f, Y: %f, state: %d\n", vy,x,y ,state);

}

void CGoomba::Render()
{
	InitAnimations();
	CAnimation* ani = this->animations["Walk"];

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	float l, t, b, r;
	GetBoundingBox(l, t, r, b);

	CGameObject::SetFlipOnNormal(nx);
	if (state == GOOMBA_STATE_DIE) {
		ani = this->animations["Die"];
		ani->Render(x - camera->GetX() + (r - l) / 2, y - camera->GetY() + GOOMBA_DIE_Y + (b - t) / 2, flip);
	}
	else
		ani->Render(x - camera->GetX() + (r - l) / 2, y - camera->GetY() + (b - t) / 2, flip);

	RenderBoundingBox();
}

bool CGoomba::CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy)
{
	if(obj->GetObjectType()==RedGoomba::ObjectType)
		return true;
}

void CGoomba::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT> coEvents,
	vector<LPCOLLISIONEVENT>& coEventsResult)
{
	coEvents.clear();

	if (state != GOOMBA_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		//DebugOut(L"coevents=0\n");
		CGameObject::UpdatePosition();
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;

		float rdx = 0;
		float rdy = 0;
		//DebugOut(L"filtercollision now =0\n");
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = -vx;
		if (ny != 0) vy = 0;

	}

}

void CGoomba::BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult)
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
				if (state!= GOOMBA_STATE_DIE) {
					SetState(GOOMBA_STATE_DIE);
				}
			}
		}
		break;

		case FireBall::ObjectType:
		{
			FireBall* fireball = dynamic_cast<FireBall*>(e->obj);
			if (e->ny != 0 || e->nx != 0)
			{
				if (state != GOOMBA_STATE_DIE) {
					SetState(GOOMBA_STATE_DIE);
					SetAlive(0);
				}
				DebugOut(L" GOOMBA DEAD by FIREBALL\n");
			}
		}
		break;

		case CGoomba::ObjectType:
		{
			CGoomba* gb = dynamic_cast<CGoomba*>(e->obj);
			if (e->nx != 0)
			{
				gb->nx = -gb->nx;
			}
		}
		break;
		
		}
	}
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			vx = 0;
			vy = 0;
			break;
	}
}

int CGoomba::GetObjectType()
{
	return ObjectType;
}
