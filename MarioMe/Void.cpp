#include "Void.h"
#include "PlayerData.h"
#include "Mario.h"

Void::Void() {
	vx = vy = 0;
	dx = dy = 0;
	gravity = 0;

	this->dx = 0;
	this->dy = 0;
}
void Void::Update(DWORD dt)
{
	
}

bool Void::CanGetThrough(CGameObject* gameObjToCollide, float coEventNx, float coEventNy)
{
	if (gameObjToCollide->GetObjectType() != CMario::ObjectType)
		return true;
	else return false;
}

void Void::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
}

void Void::BehaviorUpdate(DWORD dt)
{
	PlayerData* pd = PlayerData::GetInstance();

	float min_tx, min_ty, nx = 0, ny;
	float rdx = 0;
	float rdy = 0;

	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		switch (e->obj->GetObjectType()) {

		case CMario::ObjectType:
		{
			CMario* mario = dynamic_cast<CMario*>(e->obj);
			if (e->ny < 0)
			{
				if (mario->state != MARIO_STATE_DIE)
					mario->SetState(MARIO_STATE_DIE);
				EffectVault::GetInstance()->AddEffect(new MarioDieFx(mario->x, mario->y));
			}
		}
		break;
		}
	}
}

void Void::Render()
{
}

int Void::GetObjectType()
{
	return ObjectType;
}
