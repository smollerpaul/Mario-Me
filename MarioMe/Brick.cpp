#include "Brick.h"
#include "Game.h"
#include "Camera.h"
#include "SlidingShell.h"
#include "Mario.h"
#include "StarWhipTail.h"
#include "Coin.h"


CBrick::CBrick()
{
	width = height = 48;
	alive = 1;
}

void CBrick::Render()
{
	if (alive == 0)
		return;
	InitAnimations();
	LPANIMATION ani = this->animations["Default"];
	
	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	ani->Render(x- camera->GetX()+ width / 2, y-camera->GetY() + height/2, flip);
	//RenderBoundingBox();
}

void CBrick::InitAnimations()
{
	if (this->animations.size() < 1) {
		this->animations["Default"] = CAnimations::GetInstance()->Get("ani-brick");
	}
}

void CBrick::Update(DWORD dt)
{
	int pp=CGame::GetInstance()->GetCurrentScene()->pSwitchActivated;

	if (pp == 1) {
		SetAlive(0);
		Coin* cc = new Coin();
		cc->SetPosition(x, y);
		CGame::GetInstance()->GetCurrentScene()->AddObject(cc);
	}
}

void CBrick::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	coEvents.clear();
	if(this)
		CalcPotentialCollisions(coObjects, coEvents);
}

void CBrick::BehaviorUpdate(DWORD dt)
{
	if (coEvents.size() != 0) {
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
	}
	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		switch (e->obj->GetObjectType()) {

		case CMario::ObjectType:
		{
			CMario* mario = dynamic_cast<CMario*>(e->obj);
			if (e->ny < 0)
			{
				SetAlive(0);
				//effects
			}
		}
		break;

		case RacoonTail::ObjectType:
		{
			RacoonTail* mario = dynamic_cast<RacoonTail*>(e->obj);
			if (e->nx!=0)
			{
				SetAlive(0);
				//effects
			}
		}
		break;

		case SlidingShell::ObjectType:
		{
			SlidingShell* ss = dynamic_cast<SlidingShell*>(e->obj);
			if (e->nx != 0)
			{
				if (e->nx != 0)
				{
					SetAlive(0);
					//effects
				}
			}
		}
		break;
		}
	}
}

int CBrick::GetObjectType()
{
	return ObjectType;
}
