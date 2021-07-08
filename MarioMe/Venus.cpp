#include "Venus.h"
#include "Mario.h"
#include "Game.h"
#include "Camera.h"

Venus::Venus()
{
	width = VENUS_WIDTH;
	height = VENUS_HEIGHT;
}

void Venus::SetPosition(float xPos, float yPos)
{
	this->x = xPos;
	this->y = yPos;

	beginY = yPos;
}

void Venus::Update(DWORD dt)
{
	if (scriptStep == 3) {
		waitTimer += dt;
		if (waitTimer >= VENUS_WAIT_TIME) {
			waitTimer = 0;
			scriptStep = 0;
		 }
	}
	//set some thing venus can go down wwhen shot and ur gud
	if (scriptStep == 1) {
		revealTimer += dt;

		if (revealTimer >= VENUS_REVEAL_TIME) {
			y += dt * VENUS_SPEED;
			SetState(VENUS_HEAD_DOWN);
			if (y >=beginY) {
				y = beginY ;
				scriptStep = 3;
				revealTimer = 0;
			}
		}
	}
	
	if (scriptStep == 0) {
		SetState(VENUS_HEAD_UP);
		y -= dt* VENUS_SPEED;

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
	//DebugOut(L"Current Script Step: %d\n", scriptStep);
}

void Venus::Render()
{
	InitAnimations();
	CAnimation* ani = this->animations["RevealHeadUp"];

	if (state == VENUS_HEAD_DOWN)
		ani = this->animations["RevealHeadDown"];

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	float l, t, b, r;
	GetBoundingBox(l, t, r, b);
	ani->Render(x - camera->GetX() + (r - l) / 2, y - camera->GetY() + (b - t) / 2, flip);

	RenderBoundingBox();
}

bool Venus::CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy)
{
	if (obj->GetObjectType() == CMario::ObjectType)
		return true;
	else return false;
}

void Venus::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	if(coEvents.size()!=0)
		DebugOut(L"hello coEventSize: %d\n", coEvents.size());

	//venus wont react to collision
}

void Venus::BehaviorUpdate(DWORD dt)
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
			if (e->nx != 0 || e->ny!=0)
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
				if (scriptStep!=2) {
					scriptStep = 2;
				}
				EffectVault::GetInstance()->AddEffect(new StarWhipTail(x, y + 20));
			}
		}
		break;
		}
	}

}

void Venus::InitAnimations()
{
	if (this->animations.size() < 1) {
		this->animations["RevealHeadUp"] = CAnimations::GetInstance()->Get("ani-green-venus-fire-trap-headup")->Clone();
		this->animations["RevealHeadDown"] = CAnimations::GetInstance()->Get("ani-green-venus-fire-trap-headdown")->Clone();
		this->animations["IdleHeadUp"] = CAnimations::GetInstance()->Get("ani-green-venus-fire-trap-headup-idle")->Clone();
		this->animations["IdleHeadDown"] = CAnimations::GetInstance()->Get("ani-green-venus-fire-trap-headdown-idle")->Clone();
	}
}

int Venus::GetObjectType()
{
    return ObjectType;
}
