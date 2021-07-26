#include "MusicNote.h"
#include "PlayScene.h"
#include "EffectVault.h"
#include "Camera.h"
#include "Game.h"
#include "Mario.h"
#include "MarioConstants.h"
#include "RedSlidingShell.h"

MusicNote::MusicNote()
{
	vx = vy = 0;
	dx = dy = 0;
	gravity = 0;
	width = height = 48;
}

void MusicNote::InitAnimations()
{
	if (this->animations.size() < 1) {
		this->animations["Default"] = CAnimations::GetInstance()->Get("ani-music-note-block");
	}
}

void MusicNote::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
	this->beginY = y;
	this->beginX = x;
}

void MusicNote::Render()
{
	InitAnimations();

	LPANIMATION ani = this->animations["Default"];

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	ani->Render(x - camera->GetX() + width / 2, y - camera->GetY() + height / 2, flip);
}

void MusicNote::Update(DWORD dt)
{
	if (vy != 0) {
		yLength += abs(vy * dt);
		if (yLength >= 50) {
			vy = -vy;

			if (abs(y - beginY) <= 48) {
				vy = 0;
				y=beginY;
				yLength = 0;
			}
		}
	}

	if (vx != 0) {
		xLength += abs(vx * dt);
		if (xLength >= 50) {
			vx = -vx;

			if (abs(x - beginX) <= 48) {
				vx = 0;
				x = beginX;
				xLength = 0;
			}
		}
	}

	
	CGameObject::Update(dt);
}

void MusicNote::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
}

void MusicNote::BehaviorUpdate(DWORD dt)
{
	PlayerData* pd = PlayerData::GetInstance();

	if (coEvents.size() == 0) {
		CGameObject::UpdatePosition();
	}

	if (coEvents.size() != 0) {
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		/*x += min_tx * dx;
		y += min_ty * dy;*/
	}

	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		switch (e->obj->GetObjectType()) {

		case CMario::ObjectType:
		{
			CMario* mario = dynamic_cast<CMario*>(e->obj);
			if (e->ny != 0)
			{
				if (e->ny > 0)
					vy = 0.2;
				if (e->ny < 0)
					vy = -0.2;
			}
		}
		break;

		case RedSlidingShell::ObjectType:
		{
			RedSlidingShell* shell = dynamic_cast<RedSlidingShell*>(e->obj);
			if (e->nx != 0)
			{
				vx = -0.2;
			}
		}
		break;
		}
	}
}

void MusicNote::SetSpecialPortal(int special)
{
	specialPortal = 1;
}

int MusicNote::GetSpecialPortal()
{
	return specialPortal;
}

int MusicNote::GetObjectType()
{
	return ObjectType;
}

