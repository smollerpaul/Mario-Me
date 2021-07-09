#include "Piranha.h"
#include "Venus.h"
#include "Game.h"
#include "Camera.h"
#include "Mario.h"

Piranha::Piranha()
{
	width = PIR_WIDTH;
	height = PIR_HEIGHT;
	renderOrder = 90;
}

void Piranha::SetPosition(float xPos, float yPos)
{
	this->x = xPos;
	this->y = yPos;

	beginY = yPos;
}

void Piranha::Update(DWORD dt)
{
	if (scriptStep == 3) {
		waitTimer += dt;
		if (waitTimer >= VENUS_WAIT_TIME) {
			waitTimer = 0;
			scriptStep = 0;
		}
	}

	if (scriptStep == 1) {
		revealTimer += dt;

		if (revealTimer >= VENUS_REVEAL_TIME) {
			y += dt * VENUS_SPEED;
			SetState(VENUS_HEAD_DOWN);
			if (y >= beginY) {
				y = beginY;
				scriptStep = 3;
				revealTimer = 0;
			}
		}
	}

	if (scriptStep == 0) {
		SetState(VENUS_HEAD_UP);
		y -= dt * VENUS_SPEED;

		if (beginY - y >= height) {
			y = beginY - height;
			scriptStep = 1;
		}
	}

	if (scriptStep == 2) {
		y += dt * VENUS_SPEED;
		SetState(VENUS_HEAD_DOWN);

		if (y >= beginY) {
			y = beginY;
			scriptStep = 3;
			revealTimer = 0;
		}
	}
}

void Piranha::Render()
{
	InitAnimations();
	CAnimation* ani = this->animations["Reveal"];


	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	float l, t, b, r;
	GetBoundingBox(l, t, r, b);
	ani->Render(x - camera->GetX() + (r - l) / 2, y - camera->GetY() + (b - t) / 2, flip);

	//RenderBoundingBox();
}

bool Piranha::CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy)
{
	return true;
}

void Piranha::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
}

void Piranha::BehaviorUpdate(DWORD dt)
{
	float min_tx, min_ty, nx = 0, ny = 0;
	float rdx = 0;
	float rdy = 0;

	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);


	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		switch (e->obj->GetObjectType()) {

		case SlidingShell::ObjectType:
		{
			SlidingShell* ss = dynamic_cast<SlidingShell*>(e->obj);
			if (e->nx != 0 || e->ny != 0)
			{
				if (scriptStep != 2) {
					scriptStep = 2;
				}
			}
		}
		break;

		case FireBall::ObjectType:
		{
			FireBall* fireball = dynamic_cast<FireBall*>(e->obj);
			if (e->ny != 0 || e->nx != 0)
			{
				if (scriptStep != 2) {
					scriptStep = 2;
				}
			}
		}
		break;

		case RacoonTail::ObjectType:
		{
			RacoonTail* tail = dynamic_cast<RacoonTail*>(e->obj);
			if (e->ny != 0 || e->nx != 0)
			{
				if (scriptStep != 2) {
					scriptStep = 2;
				}
				EffectVault::GetInstance()->AddEffect(new StarWhipTail(x, y + 20));
			}
		}
		break;
		}
	}
}

void Piranha::InitAnimations()
{
	if (this->animations.size() < 1) {
		this->animations["Reveal"] = CAnimations::GetInstance()->Get("ani-green-piranha-plant-attack")->Clone();
	}
}

int Piranha::GetObjectType()
{
	return ObjectType;
}
