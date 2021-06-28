#include "Shell.h"
#include "Koopas.h"
#include "EnemiesConstants.h"
#include "Game.h"
#include "Camera.h"
#include "FireBall.h"
#include "Mario.h"
#include "Utils.h"
#include "SlidingShell.h"

ShelledKoopas::ShelledKoopas()
{
}

ShelledKoopas::ShelledKoopas(CKoopas* master)
{
	this->master = master;
	master->vx = 0;
	master->width = master->height = KOOPAS_SHELL_SIZE;
}

void ShelledKoopas::InitAnimations()
{
	if (this->animations.size() < 1 ) {
		this->animations["Crouch"] = CAnimations::GetInstance()->Get("ani-green-koopa-troopa-crouch")->Clone();
		this->animations["Respawn"] = CAnimations::GetInstance()->Get("ani-green-koopa-troopa-respawning")->Clone();

		/*if (this->flip) {
			this->animations["Move"]->GetTransform()->Scale.y = -1;
		}*/
	}
}

void ShelledKoopas::Update(DWORD dt)
{
	crouchTime += dt;
	//doi thanh normal koopas
	if (crouchTime >= KOOPAS_CROUCH_TIME) {
		DebugOut(L"Changing to walking state\n");
		flicker = 1;
	}
	
	if (flicker == 1) {
		flickerTime += dt;

		if (flickerTime >= KOOPAS_RESPAWN_TIME)
			master->SetObjectState(new NormalKoopas(master));
	}
}

void ShelledKoopas::Render()
{
	InitAnimations();
	LPANIMATION ani = this->animations["Crouch"];

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();

	master->SetFlipOnNormalEnemy(master->nx);
	int flip = master->flip;

	float l, t, b, r;
	master->GetBoundingBox(l, t, r, b);

	float mx, my;
	master->GetPosition(mx, my);

	if (flicker == 1) {
		ani = this->animations["Respawn"];
	}

	ani->Render(mx - camera->GetX() + (r - l) / 2, my - camera->GetY() + (b - t) / 2, flip);
	

}

void ShelledKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = master->x;
	top = master->y;
	right = master->x + KOOPAS_SIZE;
	bottom = master->y + KOOPAS_SIZE;
}

void ShelledKoopas::BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult, vector<LPCOLLISIONEVENT> coEvents)
{
	NormalKoopas::PostCollisionUpdate(dt, coEventsResult, coEvents);

	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		switch (e->obj->GetObjectType()) {
		case CMario::ObjectType:
		{
			CMario* mario = dynamic_cast<CMario*>(e->obj);

			if (e->ny > 0)
			{
				master->SetObjectState(new SlidingShell(master));
				DebugOut(L"Normal shell to spinning shell!!!\n");
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
				DebugOut(L"koopas killed by fireball \n");
			}
		}
		break;

		}
	}
}
