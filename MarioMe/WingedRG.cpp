#include "WingedRG.h"
#include "Mario.h"
#include "FireBall.h"
#include "GameObject.h"
#include "RedGoomba.h"
#include "NormalRG.h"
#include "EnemiesConstants.h"
#include "Game.h"
#include "Camera.h"


WingedRG::WingedRG(RedGoomba* masterObj)
{
	this->master = masterObj;
	master->SetState(RG_STATE_WALK);
	master->vx = master->GetNormalX() * RG_SPEED;
	master->vy =GRAVITY;

}

void WingedRG::InitAnimations()
{
	if (this->animations.size() < 1) {
		this->animations["WalkFly"] = CAnimations::GetInstance()->Get("ani-red-para-goomba-walk");
		this->animations["Fly"] = CAnimations::GetInstance()->Get("ani-red-para-goomba-fly");

		DebugOut(L"Done init ani WINGED\n");
	}
}

void WingedRG::Update(DWORD dt)
{
	master->dt = dt;
	master->y += master->vy * dt;

	master->vy += master->gravity * dt;
	DebugOut(L"Updated WINGED\n");

}

void WingedRG::Render()
{
	InitAnimations();
	LPANIMATION ani = this->animations["Walk"];

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();

	master->SetFlipOnNormal(master->nx);
	int flip = master->flip;

	float l, t, b, r;
	master->GetBoundingBox(l, t, r, b);

	float mx, my;
	master->GetPosition(mx, my);

	DebugOut(L"before rendering WINGED RED GOOMBA\n");

	ani->Render(mx - camera->GetX() + (r - l) / 2, my - camera->GetY() + (b - t) / 2, flip);

	DebugOut(L"rendered WINGED\n");
}

void WingedRG::BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult)
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
				master->SetObjectState(new NormalRG(master));
				DebugOut(L"Switched to Normal RG!!!!!!!!!!\n");
			}
		}
		break;

		/*case FireBall::ObjectType:
		{
			FireBall* fireball = dynamic_cast<FireBall*>(e->obj);
			if (e->ny != 0 || e->nx != 0)
			{
				if (master->GetState() != RG_STATE_DIE) {
					master->SetState(RG_STATE_DIE);
				}
			}
		}
		break;*/

		}
	}
}
