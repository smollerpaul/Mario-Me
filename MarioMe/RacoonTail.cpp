#include "RacoonTail.h"
#include"MarioConstants.h"
#include "Camera.h"

RacoonTail::RacoonTail()
{
}

RacoonTail::RacoonTail(CGameObject* fplayer)
{
	this->player = fplayer;

	//fix left side tail pos
	float pl = 0, pt = 0, pr = 0, pb = 0;
	player->GetBoundingBox(pl, pt, pr, pb);

	nx = player->GetDirection();
	width = height = TAIL_SIZE;

	if (nx > 0) 
		x = pr;
	else {
		x = pl - width;
	}

	y = pt + FIREBALL_START_Y;
}

void RacoonTail::Update(DWORD dt)
{
	aliveTimer += dt;
	if (aliveTimer >= MARIO_ATTACK_TIME) {
		aliveTimer = 0;
		SetAlive(0);
	}
}

void RacoonTail::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	coEvents.clear();
	if (this)
		CalcPotentialCollisions(coObjects, coEvents);
}

void RacoonTail::BehaviorUpdate(DWORD dt)
{
	
}

void RacoonTail::Render()
{
	RenderBoundingBox();
}

int RacoonTail::GetObjectType()
{
	return ObjectType;
}
