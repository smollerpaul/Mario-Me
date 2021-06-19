#include "SlidingShell.h"
#include "NormalKoopas.h"
#include "Shell.h"
#include "Koopas.h"
#include "EnemiesConstants.h"
#include "Game.h"
#include "Camera.h"
#include "FireBall.h"
#include "Mario.h"
#include "Koopas.h"
#include "Utils.h"
#include "EnemiesConstants.h"

SlidingShell::SlidingShell()
{
}

SlidingShell::SlidingShell(CKoopas* master)
{
	this->master = master;
	master->vx = -master->nx * KOOPAS_SHELL_SPEED;
}

void SlidingShell::InitAnimations()
{
	if (this->animations.size() < 1) {
		this->animations["Crouch"] = CAnimations::GetInstance()->Get("ani-green-koopa-troopa-crouch")->Clone();
		this->animations["Respawn"] = CAnimations::GetInstance()->Get("ani-green-koopa-troopa-respawning")->Clone();

		/*if (this->flip) {
			this->animations["Move"]->GetTransform()->Scale.y = -1;
		}*/
	}
}

void SlidingShell::Update(DWORD dt)
{
	master->vx = -master->nx * KOOPAS_SHELL_SPEED;
	// lam cái đụng 1 cái độ nào đó r quay lại
	master->x += master->vx * dt;
	DebugOut(L"vx: %f\n", master->vx);
}

void SlidingShell::Render()
{
	InitAnimations();
	LPANIMATION ani = this->animations["Crouch"];

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();

	master->SetFlipOnNormalEnemy(master->nx);

	int flip = master->flip;
	
	if (flip == lastFlip && lastFlip!=0) {
		flip *= -1;
	}

	//DebugOut(L"flup: %d\n", flip);

	float l, t, b, r;
	master->GetBoundingBox(l, t, r, b);

	float mx, my;
	master->GetPosition(mx, my);

	ani->Render(mx - camera->GetX() + (r - l) / 2, my - camera->GetY() + (b - t) / 2, flip);
	lastFlip = flip;
	
}

void SlidingShell::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = master->x;
	top = master->y;
	right = master->x + KOOPAS_SIZE;
	bottom = master->y + KOOPAS_SIZE;
}

void SlidingShell::BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult)
{
	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		switch (e->obj->GetObjectType()) {
		
		case FireBall::ObjectType:
		{
			FireBall* fireball = dynamic_cast<FireBall*>(e->obj);
			if (e->ny != 0 || e->nx != 0)
			{
				if (master->GetState() != KOOPAS_STATE_DIE) {
					master->SetState(KOOPAS_STATE_DIE);
				}
				master->SetAlive(0);
				DebugOut(L"koopas killed by fireball \n");
			}
		}
		break;

		}
	}
}
