#include "Coin.h"
#include "Camera.h"
#include "Game.h"
#include "Mario.h"

Coin::Coin()
{
	vx = vy = 0;
	dx = dy = 0;
	gravity = 0;

	width = height = COIN_SIZE;
	SetState(COIN_STATE_ACTIVE);
}

void Coin::InitAnimations()
{
	if (this->animations.size() < 1) {
		this->animations["CoinDefault"] = CAnimations::GetInstance()->Get("ani-coin");
		this->animations["CoinFreeze"] = CAnimations::GetInstance()->Get("ani-idle-coin");
		this->animations["BricksDefault"] = CAnimations::GetInstance()->Get("ani-brick");
		this->animations["BricksFreeze"] = CAnimations::GetInstance()->Get("ani-brick-time-freeze");
	}
}

void Coin::Render()
{
	InitAnimations();
	CAnimation* ani = this->animations["CoinDefault"];

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	float l, t, b, r;
	GetBoundingBox(l, t, r, b);

	ani->Render(x - camera->GetX() + (r - l) / 2, y - camera->GetY() + (b - t) / 2, flip);
}

void Coin::Update(DWORD dt)
{
	CGameObject::Update(dt);
	CGameObject::UpdatePosition();
}

bool Coin::CanGetThrough(CGameObject* gameObjToCollide, float coEventNx, float coEventNy)
{
	if (gameObjToCollide->GetObjectType() == CMario::ObjectType) {
		return true;
	}
	return false;
}

void Coin::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	coEvents.clear();
	if (state != COIN_STATE_ACTIVE)
		return;

	CalcPotentialCollisions(coObjects, coEvents);
}

void Coin::BehaviorUpdate(DWORD dt)
{
	PlayerData* pd = PlayerData::GetInstance();

	if (coEvents.size() != 0) {
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		if (nx != 0) {
			SetAlive(0);
			pd->SetCoins(pd->GetCoins() + 1);
		}
			
		if (ny != 0) {
			SetAlive(0);
			pd->SetCoins(pd->GetCoins() + 1);
		}
	}
	
	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		switch (e->obj->GetObjectType()) {
		case CMario::ObjectType: {
			CMario* p = dynamic_cast<CMario*>(e->obj);

			if (e->nx!= 0|| e->ny != 0) {
				//
			}
		}
		break;
		}
	}
}

int Coin::GetObjectType()
{
	return ObjectType;
}
