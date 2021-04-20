#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"
#include "Animations.h"

#include "Goomba.h"
#include "Portal.h"
#include "Keyboard.h"

CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_BIG;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	flip = 1;

	start_x = x; 
	start_y = y; 

	this->x = x; 
	this->y = y; 
	//nx<0 thi flip=-1
}


void CMario::InitAnimations()
{
	if (this->animations.size() < 1 ) {
		this->animations["Idle"] = CAnimations::GetInstance()->Get("ani-big-mario-idle");
		this->animations["Walk"] = CAnimations::GetInstance()->Get("ani-big-mario-walk");
		this->animations["Run"] = CAnimations::GetInstance()->Get("ani-big-mario-run");
		this->animations["HighSpeed"] = CAnimations::GetInstance()->Get("ani-big-mario-high-speed");
		this->animations["Jump"] = CAnimations::GetInstance()->Get("ani-big-mario-jump");
		this->animations["HighJump"] = CAnimations::GetInstance()->Get("ani-big-mario-high-jump");
		this->animations["Fly"] = CAnimations::GetInstance()->Get("ani-big-mario-high-jump");
		this->animations["Fall"] = CAnimations::GetInstance()->Get("ani-big-mario-fall");
		this->animations["Float"] = CAnimations::GetInstance()->Get("ani-big-mario-high-jump");
		this->animations["Skid"] = CAnimations::GetInstance()->Get("ani-big-mario-skid");
		this->animations["Crouch"] = CAnimations::GetInstance()->Get("ani-big-mario-crouch");
		this->animations["Kick"] = CAnimations::GetInstance()->Get("ani-big-mario-kick");

		this->animations["Hold"] = CAnimations::GetInstance()->Get("ani-big-mario-hold");
		this->animations["HoldIdle"] = CAnimations::GetInstance()->Get("ani-big-mario-hold-idle");
		this->animations["HoldFall"] = CAnimations::GetInstance()->Get("ani-big-mario-hold-fall");

		this->animations["TeleVer"] = CAnimations::GetInstance()->Get("ani-big-mario-idle-front");
		this->animations["TeleHor"] = CAnimations::GetInstance()->Get("ani-big-mario-walk");
	}
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	ResetFlip();
	Keyboard* keyboard = CGame::GetInstance()->GetKeyboard();
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += MARIO_GRAVITY*dt;
	if (keyboard->IsKeyDown(DIK_RIGHT)) {
		SetState(MARIO_STATE_WALKING_RIGHT);
	}

	if (keyboard->IsKeyDown(DIK_LEFT)) {
		SetState(MARIO_STATE_WALKING_LEFT);
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state!=MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	DWORD now = GetTickCount64();
	if ( now - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		ResetUntouchable();
	}

	// No collision occured, proceed normally
	if (coEvents.size()==0)
	{
		x += dx; 
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0; 
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 
		
		// block every object first!
		x += min_tx*dx + nx*0.4f;
		y += min_ty*dy + ny*0.4f;

		if (nx!=0) vx = 0;
		if (ny!=0) vy = 0;


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState()!= GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable==0)
					{
						if (goomba->GetState()!=GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else 
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			} // if Goomba
			else if (dynamic_cast<CPortal *>(e->obj))
			{
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMario::Render()
{
	InitAnimations();
	CAnimation* ani = this->animations["Idle"];

	CGameObject::SetAnimationFlip(nx);

	//if (state == MARIO_STATE_DIE)
		//ani = MARIO_ANI_DIE;
	//else
		if (level == MARIO_LEVEL_BIG)
		{
			if (vx == 0) {
				ani = this->animations["Idle"];
			}
			else {
				ani = this->animations["Walk"];
			}

		}
	
	/*if (level == MARIO_LEVEL_SMALL)
	{
		if (vx == 0) {
			ani = MARIO_ANI_SMALL_IDLE_RIGHT;
		}
		else {
			ani = MARIO_ANI_SMALL_WALKING_RIGHT;
		}
	}*/

	int alpha = 255;
	if (untouchable) alpha = 128;


	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	ani->Render(x- camera->GetX(), y-camera->GetY(), flip, alpha);

	RenderBoundingBox();
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		vx = MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT: 
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		vy = -MARIO_JUMP_SPEED_Y;
		break; 
	case MARIO_STATE_IDLE: 
		vx = 0;
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	}
}

void CMario::OnKeyUp(int keyCode)
{
	if (state != MARIO_STATE_IDLE)
		SetState(MARIO_STATE_IDLE);
}

void CMario::OnKeyDown(int keyCode)
{
	switch (keyCode)
	{
	case DIK_SPACE:
		SetState(MARIO_STATE_JUMP);
		break;
	case DIK_A:
		Reset();
		break;
	}
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y; 

	if (level==MARIO_LEVEL_BIG)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	else
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
	ResetUntouchable();
	ResetFlip();
}

void CMario::ResetFlip()
{
	if (flip != 1)
		flip = 1;
}

