#include "Utils.h"

#include "Mario.h"
#include "Game.h"
#include "Animations.h"

#include "Goomba.h"
#include "Portal.h"
#include "Keyboard.h"
#include "Camera.h"
#include "PlayerData.h"
#include "SolidBlock.h"

CMario::CMario() : CGameObject()
{
	//currently using level big only
	switch (PlayerData::GetInstance()->GetMarioType()) {
	case 1:
		level = MARIO_LEVEL_SMALL;
		
		break;
	case 2:
		level = MARIO_LEVEL_BIG;
		SetSize(MARIO_BIG_BBOX_WIDTH, MARIO_BIG_BBOX_HEIGHT);
		
		break;
	}
	
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	flip = 1;

	this->x = 0; 
	this->y = 0; 

	this->gravity = MARIO_GRAVITY;

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

		DebugOut(L"done init ani\n");
	}
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	// reset untouchable timer if untouchable time has passed
	DWORD now = GetTickCount64();

	if (now - untouchable_start > MARIO_UNTOUCHABLE_TIME)
		ResetUntouchable();
	
	vy += gravity * dt;

	
	MovementUpdate(dt);
	CollisionUpdate(dt, coObjects, coEvents, coEventsResult);
	BehaviorUpdate(dt, coEventsResult);

	
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];	
}

void CMario::MovementUpdate(DWORD dt)
{
	ResetFlip();
	Keyboard* keyboard = CGame::GetInstance()->GetKeyboard();
	
	CGameObject::Update(dt);

	/*	dang run, walk -> cant crouch
		dang idle -> can crouch
		dang crouch -> can run, walk 
	*/ 

	if(keyboard->IsKeyDown(DIK_DOWN)) {
		if (state != MARIO_STATE_WALK && state != MARIO_STATE_RUN && state != MARIO_STATE_CROUCH) 
			SetState(MARIO_STATE_CROUCH);

		if (state == MARIO_STATE_CROUCH) {
			DebugOut(L"IM HEREEEEEEEEEEE here\n");
			if (vx != 0)
				vx = 0;

			SetSize(MARIO_BIG_BBOX_WIDTH, MARIO_BIG_BBOX_HEIGHT - MARIO_CROUCH_SUBSTRACT);
			
		}
	}

	if (keyboard->IsKeyDown(DIK_RIGHT) || keyboard->IsKeyDown(DIK_LEFT)) {
		
		nx = keyboard->IsKeyDown(DIK_RIGHT) ? 1 : -1;

		if (isOnGround) {

			if (state == MARIO_STATE_CROUCH) {
				float currentX, currentY;
				GetPosition(currentX, currentY);

				SetPosition(currentX, currentY - 2 * MARIO_CROUCH_SUBSTRACT);
				SetSize(MARIO_BIG_BBOX_WIDTH, MARIO_BIG_BBOX_HEIGHT);
			}
			if (state!= MARIO_STATE_WALK) {
				SetState(MARIO_STATE_WALK);
				//vx = nx * MARIO_WALK_ACCELERATION;
			}
			
			if (state == MARIO_STATE_WALK) {
				vx += nx* MARIO_WALK_ACCELERATION * dt;

				if (vx >= MARIO_WALK_SPEED || vx <= -MARIO_WALK_SPEED) {
					vx = nx * MARIO_WALK_SPEED;
				}
			}

			if (keyboard->IsKeyDown((DIK_A))) {
				if (state != MARIO_STATE_RUN) {
					SetState(MARIO_STATE_RUN);
				}
					//nx == 1 ? vx += MARIO_RUN_ACCELERATION * dt : vx -= MARIO_RUN_ACCELERATION * dt;

					//if (vx >= MARIO_RUN_SPEED || vx <= -MARIO_RUN_SPEED) {
						vx = nx * MARIO_RUN_SPEED;
					//}
				
			}

			
			/*if (state == MARIO_STATE_WALK) {
				nx = 1 ? vx += MARIO_WALK_ACCELERATION : vx -= -MARIO_WALK_ACCELERATION;

				if (vx >= MARIO_RUN_SPEED || vx <= -MARIO_RUN_SPEED) {
					SetState(MARIO_STATE_RUN);
					vx = nx * MARIO_RUN_SPEED;
				}
			}*/
		}
	}
	

	DebugOut(L"[MARIO SPEED]:  posX: %f, posY: %f, width: %f, height: %f\n", x, y,width, height);
}

void CMario::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects, 
	vector<LPCOLLISIONEVENT> coEvents, vector<LPCOLLISIONEVENT> &coEventsResult)
{
	coEvents.clear();
	
	//turn off collision when die
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	// k co su kien va cham nao <=> k cham gi ca <=> coEvents#size = 0
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;


		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		isOnGround = true;
	}
	
}

void CMario::BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult)
{
	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		switch (e->obj->GetObjectType()) {

			case CGoomba::ObjectType: 
				{
					CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

					if (e->ny < 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							goomba->SetState(GOOMBA_STATE_DIE);
							vy = -MARIO_JUMP_DEFLECT_SPEED;
						}
					}
					else if (e->nx != 0)
					{
						if (untouchable == 0)
						{
							if (goomba->GetState() != GOOMBA_STATE_DIE)
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
				}
				break;

			case CPortal::ObjectType: 
				{
					CPortal* p = dynamic_cast<CPortal*>(e->obj);
					CGame::GetInstance()->SwitchScene(p->GetSceneId());
				}
				break;

				case SolidBlock ::ObjectType:
				{
					SolidBlock* s = dynamic_cast<SolidBlock*>(e->obj);
					        
					if (e->nx != 0) {
						SetState(MARIO_STATE_IDLE);
					}
				}
				break;
		}
	}
}


void CMario::Render()
{
	InitAnimations();
	
	CAnimation* ani = this->animations["Idle"];

	CGameObject::SetAnimationFlip(nx);

		if (level == MARIO_LEVEL_BIG)
		{
			switch (this->state) {
			case MARIO_STATE_IDLE:
				ani = this->animations["Idle"];
				break;

			case MARIO_STATE_WALK:
				ani = this->animations["Walk"];
				break;

			case MARIO_STATE_RUN:
				ani = this->animations["HighSpeed"];
				break;
			case MARIO_STATE_CROUCH:
				ani = this->animations["Crouch"];
				break;
			}
			
		}

	int alpha = 255;
	if (untouchable) 
		alpha = 128;


	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	ani->Render(x - camera->GetX(), y - camera->GetY(), flip, alpha);

	RenderBoundingBox();
	/*DebugOut(L"aniiiiiiiiiiiiiiiiiiii of mario : %s\n" );*/

	//DebugOut(L"ssssssssssssssssssssssize of mario : w  %f, h  %f\n", width, height);
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
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
	if (state == MARIO_STATE_CROUCH) {
		float currentX, currentY;
		GetPosition(currentX, currentY);

		SetPosition(currentX, currentY - MARIO_CROUCH_SUBSTRACT);
		SetSize(MARIO_BIG_BBOX_WIDTH, MARIO_BIG_BBOX_HEIGHT);

	}	

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
		/*case DIK_A:
			Reset();
			break;*/

	case DIK_DOWN:
	{
		DebugOut(L"Get in here\n");
		float currentX, currentY;
		GetPosition(currentX, currentY);
		SetPosition(currentX, currentY + MARIO_CROUCH_SUBSTRACT);

		SetState(MARIO_STATE_CROUCH);
	}
	break;

	}
}


void CMario::ResetUntouchable()
{
	untouchable = 0; 
	untouchable_start = 0;
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



bool CMario::GetIsOnGround()
{
	return isOnGround;
}

void CMario::StartUntouchable()
{
	untouchable = 1;
	untouchable_start = GetTickCount64();
}


