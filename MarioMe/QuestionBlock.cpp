#include "QuestionBlock.h"
#include "Camera.h"
#include "Game.h"
#include "Mario.h"

//BOUNCING ani -> coint effect

QuestionBlock::QuestionBlock()
{
	vx = vy = 0;
	dx = dy = 0;
	gravity = 0;

	width = height = QB_SIZE;
	SetState(QB_STATE_ACTIVE);
}

void QuestionBlock::InitAnimations()
{
	if (this->animations.size() < 1) {
		this->animations["Active"] = CAnimations::GetInstance()->Get("ani-question-block");
		this->animations["Frozen"] = CAnimations::GetInstance()->Get("ani-empty-block");
	}
}

void QuestionBlock::SetPosition(float x, float y) {
	this->x = x;
	this->y = y;
	yStill = y;
}

void QuestionBlock::Render()
{
	InitAnimations();
	CAnimation* ani = this->animations["Active"];

	if(state==QB_STATE_FROZEN || state == QB_STATE_BOUNCE)
		ani = this->animations["Frozen"];

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	float l, t, b, r;
	GetBoundingBox(l, t, r, b);

	ani->Render(x - camera->GetX() + (r - l) / 2, y - camera->GetY() + (b - t) / 2, flip);

	RenderBoundingBox();
}

void QuestionBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	// bounce and then drop to original position
	

	if (state == QB_STATE_BOUNCE) {
		dyBounce += abs(dy);
		if (dyBounce >= QB_BOUNCE_HEIGHT) {
			vy += QB_GRAVITY * dt;
		}

		if (y > yStill) {
			vy = 0;
			y = yStill;
			SetState(QB_STATE_FROZEN);
		}
	}
	CGameObject::Update(dt, coObjects);
	CGameObject::UpdatePosition();

	CollisionUpdate(dt, coObjects, coEvents, coEventsResult);
	BehaviorUpdate(dt, coEventsResult);

	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}

bool QuestionBlock::CanGetThrough(CGameObject* gameObjToCollide, float coEventNx, float coEventNy)
{
	return false;
}

void QuestionBlock::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT> coEvents, vector<LPCOLLISIONEVENT>& coEventsResult)
{
	coEvents.clear();
	if (state != QB_STATE_ACTIVE)
		return;

	CalcPotentialCollisions(coObjects, coEvents);

	float min_tx, min_ty, nx = 0, ny;
	float rdx = 0;
	float rdy = 0;

	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
}

void QuestionBlock::BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult)
{
	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		switch (e->obj->GetObjectType()) {

		case CMario::ObjectType:
		{
			CMario* mario = dynamic_cast<CMario*>(e->obj);
			if (e->ny < 0)
			{
				vy = -QB_BOUNCE_SPEED ;
				SetState(QB_STATE_BOUNCE);
			}
		}
		break;

		}
	}
}

int QuestionBlock::GetObjectType()
{
    return ObjectType;
}
