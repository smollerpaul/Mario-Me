#include "PSwitch.h"
#include "Game.h"
#include "Camera.h"
#include "Mario.h"

PSwitch::PSwitch()
{
	width = height = PSWITCH_SIZE;
	visible = 1;
}

void PSwitch::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;

	if (activated == 1) {
		t = y + 20;
	}
}

void PSwitch::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;

	beginY = y;
}

void PSwitch::Render()
{
	InitAnimations();
	if (visible == 0)
		return;
	LPANIMATION ani = this->animations["Default"];

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	float l, t, b, r;
	GetBoundingBox(l, t, r, b);

	if (activated == 1) {
		ani = this->animations["Pressed"];
		ani->Render(x - camera->GetX() + (r - l) / 2, y +20- camera->GetY() + (b - t) / 2, flip);
		return;
	}
	
	ani->Render(x - camera->GetX() + (r - l) / 2, y - camera->GetY() + (b - t) / 2, flip);	
}

void PSwitch::InitAnimations()
{
	this->animations["Default"] = CAnimations::GetInstance()->Get("ani-switch-block");
	this->animations["Pressed"] = CAnimations::GetInstance()->Get("ani-switch-block-pressed");
}

void PSwitch::Update(DWORD dt)
{
	if (scriptStep == 0) {
		y -= 0.2 * dt;
		if (beginY - y >= height) {
			y = beginY - height;
			scriptStep = 1;
		}
	}
	
}

void PSwitch::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	/*coEvents.clear();
	if(this)
		CalcPotentialCollisions(coObjects, coEvents);*/
}

void PSwitch::BehaviorUpdate(DWORD dt)
{
	/*PlayerData* pd = PlayerData::GetInstance();

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
			if (e->ny > 0)
			{
				activated = 1;
				CGame::GetInstance()->GetCurrentScene()->pSwitchActivated = 1;
				int pp = CGame::GetInstance()->GetCurrentScene()->pSwitchActivated;
				
			}
		}
		break;
		}
	}*/
}

void PSwitch::SetVisible(int vs)
{
	visible = vs;
}

int PSwitch::GetObjectType()
{
	return ObjectType;
}
