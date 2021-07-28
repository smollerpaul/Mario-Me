#include "EndCard.h"
#include "Camera.h"
#include "Game.h"
#include "Mario.h"
#include "FlyingCard.h"

EndCard::EndCard()
{
	gravity = 0;
	width = height = 48;
	SetState(CARD_STATE_MUSHROOM);
}

void EndCard::InitAnimations()
{
	if (this->animations.size() < 1) {
		this->animations["Mushroom"] = CAnimations::GetInstance()->Get("ani-super-mushroom")->Clone();
		this->animations["Flower"] = CAnimations::GetInstance()->Get("ani-fire-flower")->Clone();
		this->animations["Star"] = CAnimations::GetInstance()->Get("ani-star-man-white")->Clone();
	}
}

void EndCard::Render()
{
	InitAnimations();
	CAnimation* ani = this->animations["Mushroom"];

	
	switch (state)
	{
	case CARD_STATE_MUSHROOM:
		ani= this->animations["Mushroom"];
		break;

	case CARD_STATE_STAR:
		ani = this->animations["Star"];
		break;

	case CARD_STATE_FLOWER:
		ani = this->animations["Flower"];
		break;

	case CARD_STATE_ACHIEVED:
		return;
		break;
	}

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	float l, t, b, r;
	GetBoundingBox(l, t, r, b);

	ani->Render(x - camera->GetX() + (r - l) / 2, y - camera->GetY() + (b - t) / 2, flip);
	RenderBoundingBox();
}

void EndCard::Update(DWORD dt)
{
	swapTime += dt;
	if (swapTime < 100) {
		SetState(CARD_STATE_MUSHROOM);
	}
	if (swapTime < 250 && swapTime > 100) {
		SetState(CARD_STATE_FLOWER);
	}
	if (swapTime < 350 && swapTime > 250) {
		SetState(CARD_STATE_STAR);
		swapTime = 0;
	}
}

bool EndCard::CanGetThrough(CGameObject* gameObjToCollide, float coEventNx, float coEventNy)
{
	return false;
}

void EndCard::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
}

void EndCard::BehaviorUpdate(DWORD dt)
{
	CGameObject::UpdatePosition();
}

int EndCard::GetObjectType()
{
	return ObjectType;
}
