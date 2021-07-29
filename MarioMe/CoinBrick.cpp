#include "CoinBrick.h"
#include "EffectVault.h"
#include "Camera.h"
#include "Game.h"
#include "Mario.h"
#include "MoneyFx.h"


CoinBrick::CoinBrick()
{
	vx = vy = 0;
	dx = dy = 0;
	gravity = 0;
	width = height = 48;
}

void CoinBrick::InitAnimations()
{
	if (this->animations.size() < 1) {
		this->animations["Default"] = CAnimations::GetInstance()->Get("ani-brick");
		this->animations["Empty"] = CAnimations::GetInstance()->Get("ani-empty-block");
	}
}

void CoinBrick::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
	this->beginY = y;
	this->beginX = x;
}

void CoinBrick::Render()
{
	InitAnimations();

	LPANIMATION ani = this->animations["Default"];

	if (hitCount == 0)
		ani = this->animations["Empty"];

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	ani->Render(x - camera->GetX() + width / 2, y - camera->GetY() + height / 2, flip);
	RenderBoundingBox();
}

void CoinBrick::Update(DWORD dt)
{
	if (vy != 0) {
		yLength += abs(vy * dt);
		if (yLength >= 50) {
			vy = -vy;

			if (abs(y - beginY) <= 48) {
				vy = 0;
				y = beginY;
				yLength = 0;
			}
		}
	}

	CGameObject::Update(dt);
}

void CoinBrick::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
}

void CoinBrick::BehaviorUpdate(DWORD dt)
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
				if (hitCount > 0) {
					vy = -0.2;
					hitCount -= 1;
					
					EffectVault::GetInstance()->AddEffect(new MoneyFx(this->x, this->y));

					pd->SetScore(pd->GetScore() + 100);
					pd->SetCoins(pd->GetCoins() + 1);
					EffectVault::GetInstance()->AddEffect(new ScoreFx("100", x, y));
				}
			}
		}
		break;
		}
	}
}

int CoinBrick::GetObjectType()
{
	return ObjectType;
}
