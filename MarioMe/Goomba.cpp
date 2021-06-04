#include "Goomba.h"
#include "Game.h"
#include "Camera.h"
#include "Mario.h"
#include "FireBall.h"

// dung fireball chet
// dung nhau thi quay huong nguoc lai
// dung chan mario chet
// dung mario thì di xuyen wa
// dung duoc tren ground

CGoomba::CGoomba()
{
	SetState(GOOMBA_STATE_WALK);
	

	width = GOOMBA_BBOX_WIDTH;
	height = GOOMBA_BBOX_HEIGHT;

	DebugOut(L"NOWWWWWWWWWW vy: %f\n", vy);
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

	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else 	
		bottom = y + height;
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CGameObject::Update(dt, coObjects);

	vy += gravity * dt;

	CGameObject::UpdatePosition();

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

	CGameObject::SetFlipOnNormal(nx);
	if (state == GOOMBA_STATE_DIE) {
		ani = this->animations["Die"];
	}
	
	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	float l, t, b, r;
	GetBoundingBox(l, t, r, b);

	ani->Render(x - camera->GetX() + (r - l) / 2, y - camera->GetY() + (b - t) / 2, flip);

	RenderBoundingBox();
}

void CGoomba::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT> coEvents,
	vector<LPCOLLISIONEVENT>& coEventsResult)
{
	coEvents.clear();

	if (state != GOOMBA_STATE_DIE) {
		CalcPotentialCollisions(coObjects, coEvents);

		if (coEvents.size() != 0) {
			//cuz its a moving object
			float min_tx, min_ty, nx = 0, ny;
			float rdx = 0;
			float rdy = 0;

			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;


			if (nx != 0) {
				vx = -vx;
			}
			if (ny != 0) vy = 0;

		}
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
					vy = -MARIO_JUMP_DEFLECT_SPEED;
				}

				DebugOut(L" GOOMBA DEAD by MARIO\n");
			}
		}
		break;

		case FireBall::ObjectType:
		{
			FireBall* fireball = dynamic_cast<FireBall*>(e->obj);

			if (e->ny != 0 || e->nx != 0)
			{
				if (state != GOOMBA_STATE_DIE)
					SetState(GOOMBA_STATE_DIE);

				DebugOut(L" GOOMBA DEAD by FIREBALL\n");
			}
		}
		break;
		case CGoomba::ObjectType:
		{
			CGoomba* gb = dynamic_cast<CGoomba*>(e->obj);

			if ( e->nx != 0)
			{
				nx *= -1;
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
			y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
			vx = 0;
			vy = 0;
			break;
		case GOOMBA_STATE_WALK: 
			vx = -GOOMBA_WALK_SPEED;
	}
}

int CGoomba::GetObjectType()
{
	return ObjectType;
}
