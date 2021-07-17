#include "StrawCube.h"
#include "Leaf.h"
#include "PlayScene.h"
#include "EffectVault.h"
#include "Camera.h"
#include "Game.h"
#include "Mario.h"
#include "MarioConstants.h"

void StrawCube::SetReward(int blockReward)
{
    this->cubeReward = blockReward;
}

int StrawCube::GetObjectType()
{
    return ObjectType;
}

StrawCube::StrawCube(CPlayScene* ss)
{
    vx = vy = 0;
    dx = dy = 0;
    gravity = 0;
    width = height = 48;
    SetState(STRAW_STATE_ACTIVE);
    this->scene = ss;
}

void StrawCube::InitAnimations()
{
	if (this->animations.size() < 1) {
		this->animations["Default"] = CAnimations::GetInstance()->Get("ani-wood-block");
		this->animations["Empty"] = CAnimations::GetInstance()->Get("ani-empty-block");
	}
}

void StrawCube::SetPosition(float x, float y)
{
    this->x = x;
    this->y = y;
    this->beginX = x;
}

void StrawCube::Render()
{
	InitAnimations();

	LPANIMATION ani = this->animations["Default"];

	if (x != beginX) {
		ani = this->animations["Empty"];
	}

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	ani->Render(x - camera->GetX() + width / 2, y - camera->GetY() + height / 2, flip);
}

void StrawCube::Update(DWORD dt)
{
	if (vx != 0 ) {
		xLength += vx * dt;
		if (abs(xLength) >= 50) {
			vx = -0.2;

			if (x-beginX <= 48) {
				vx = 0;
				x = beginX;
			}
		}
	}

	CGameObject::Update(dt);
}

void StrawCube::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    coEvents.clear();
    CalcPotentialCollisions(coObjects, coEvents);
}

void StrawCube::BehaviorUpdate(DWORD dt)
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
			if (e->nx!= 0)
			{
				if (state == STRAW_STATE_ACTIVE) {
					SetState(STRAW_STATE_EMPTY);
					if (cubeReward == LEAF_PRIZE) {
						vx = 0.2;
						Leaf* leaf = new Leaf(scene);
						leaf->SetPosition(x, y - 48);
						CGame::GetInstance()->GetCurrentScene()->AddObject(leaf);
					}

					pd->SetScore(pd->GetScore() + 100);
				}
			}
		}
		break;

		case RacoonTail::ObjectType:
		{
			RacoonTail* tail = dynamic_cast<RacoonTail*>(e->obj);
			if (e->nx != 0)
			{
				
			}
		}
		break;

		default: break;

		}
	}
}
