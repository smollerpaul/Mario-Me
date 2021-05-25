#include "Goomba.h"
#include "Game.h"
#include "Camera.h"
#include "Mario.h"

CGoomba::CGoomba()
{
	SetState(GOOMBA_STATE_WALK);
}

void CGoomba::InitAnimations()
{
	if (this->animations.size() < 1) {
		this->animations["Walk"] = CAnimations::GetInstance()->Get("ani-goomba-walk");
	}

	//DebugOut(L"Done init ani Gooomba \n");
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;

	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else 	
		bottom = y + GOOMBA_BBOX_HEIGHT;
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CGameObject::Update(dt, coObjects);

	vy += gravity * dt;
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;

	CollisionUpdate(dt, coObjects, coEvents, coEventsResult);
	BehaviorUpdate(dt, coEventsResult);

	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];

}

void CGoomba::Render()
{
	InitAnimations();
	CAnimation* ani = this->animations["Walk"];

	CGameObject::SetFlipOnNormal(nx);
	
	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	/*float l, t, b, r;
	GetBoundingBox(l, t, r, b);*/

	ani->Render(x-camera->GetX(),y-camera->GetY(),flip);
	/*ani->Render(x - camera->GetX() + (r - l) / 2, y - camera->GetY() + (b - t) / 2, flip);*/

	RenderBoundingBox();
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
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;


		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

	}

}

void CGoomba::BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult)
{
	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		switch (e->obj->GetObjectType()) {
			// when stamped by mario
		case CMario::ObjectType:
		{
			CMario* mario = dynamic_cast<CMario*>(e->obj);

			if (e->ny > 0)
			{
				if (GetState() != GOOMBA_STATE_DIE)
					SetState(GOOMBA_STATE_DIE);

				DebugOut(L" GOOMBA DEAD (state: %d )\n", state);
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
