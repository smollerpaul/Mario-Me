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

		this->animations["Spin"] = CAnimations::GetInstance()->Get("ani-raccoon-mario-spin");

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
	JumpUpdate(dt);
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

#pragma region DOWN
	if (keyboard->IsKeyDown(DIK_DOWN)) {
		if (state != MARIO_STATE_WALK && state != MARIO_STATE_RUN && state != MARIO_STATE_CROUCH )
			SetState(MARIO_STATE_CROUCH);

		if (state == MARIO_STATE_CROUCH) {
			if (vx != 0)
				vx = 0;

			SetSize(MARIO_BIG_BBOX_WIDTH, MARIO_BIG_BBOX_HEIGHT - MARIO_CROUCH_SUBSTRACT);
		}
	}
#pragma endregion

#pragma region RIGHT LEFT
	if (keyboard->IsKeyDown(DIK_RIGHT) || keyboard->IsKeyDown(DIK_LEFT)) {
		int direction = 0;

		direction = finalKeyDirection;
		
		//get up from crouch to walk
		if (state == MARIO_STATE_CROUCH) {
			float currentX, currentY;
			GetPosition(currentX, currentY);

			SetPosition(currentX, currentY - MARIO_CROUCH_SUBSTRACT);
			SetSize(MARIO_BIG_BBOX_WIDTH, MARIO_BIG_BBOX_HEIGHT);
		}

		if (isOnGround) {
			if (state != MARIO_STATE_WALK) {
				DebugOut(L"vo day walk\n");
				SetState(MARIO_STATE_WALK);
			}
		}

		float maxSpeed = MARIO_WALK_SPEED;
		accelerate_x = direction * MARIO_WALK_ACCELERATION;

		if (keyboard->IsKeyDown((DIK_A))) {

			if (state != MARIO_STATE_RUN) {
				DebugOut(L"vo day run\n");
				SetState(MARIO_STATE_RUN);
			}

			maxSpeed = MARIO_RUN_SPEED;
			accelerate_x = direction * MARIO_RUN_ACCELERATION;
		}
		

		//skid when direction is against vx
		if (vx * direction < 0 && GetIsOnGround() != 0) {
			SetSkid(1);
			accelerate_x = direction * MARIO_SKID_ACCELERATION;

			if (keyboard->IsKeyDown(DIK_A)) {
				accelerate_x = 2 * direction * MARIO_SKID_ACCELERATION;
			}

			if (!GetIsOnGround()) {
				accelerate_x = MARIO_SKID_ACCELERATION * direction * 2;
			}
		}

		vx += accelerate_x * dt;

		// when mario maxes out maxSpeed, slowly reduce to maxSpeed for smooth UX
		
		if (abs(vx) >= maxSpeed) {
			vx = direction * maxSpeed;
		}

		if (vx * direction >= 0) {
			SetSkid(0);
		}

		SetDirection(direction);
	}
	else {
		//still goes in here everytime
		SetSkid(0);

		// slow down to reach vx=0 (stop)
		if (abs(vx) > MARIO_WALK_FRICTION * dt) {
			int speedDirection = (vx > 0) ? 1 : -1;
			vx -= speedDirection * MARIO_WALK_FRICTION * dt;
		}
		else {
			vx = 0;
			// not crouch and on ground -> can only be idle	
			if (state != MARIO_STATE_CROUCH && GetIsOnGround() == true) {
				SetState(MARIO_STATE_IDLE);
			}
		}
	}

	// set normal friction (walk friction)
	if (state != MARIO_STATE_CROUCH) {
		if (state == MARIO_STATE_RUN) {
			SetFriction(MARIO_RUN_FRICTION);
		}
		else {
			SetFriction(MARIO_WALK_FRICTION);
		}
	}
	// not onGround -> no friction
	if (GetIsOnGround() == false) {
		SetFriction(0);
	}
	
#pragma endregion

#pragma region JUMP
	
}

void CMario::JumpUpdate(DWORD dt)
{
	//bi nhay giua giua bouncy bounce mixed state
	Keyboard* keyboard = CGame::GetInstance()->GetKeyboard();

	float height = 0;
	if (state == MARIO_STATE_JUMP_FALL) {
		if (GetIsOnGround() == true) {
			SetState(MARIO_STATE_IDLE);
			vy = MARIO_GRAVITY * dt;
			DebugOut(L" cuz it will never get into ground when.. falling lol \n");
		}
		
	}

	// set high jump
	if (state == MARIO_STATE_JUMP || state == MARIO_STATE_JUMP_HIGH || state == MARIO_STATE_JUMP_FALL) {
		height = abs(jumpStartPosition - y - vy * dt);
	}

	if (state != MARIO_STATE_JUMP_FALL) {
		// height < small fall point but is jumping
		if (height < MARIO_JUMP_FALL_POINT && height != 0) {
			vy = -MARIO_JUMP_PUSH - MARIO_GRAVITY * dt;
		}

		if (height > MARIO_BEGIN_HIGH_JUMP_HEIGHT && state== MARIO_STATE_JUMP) {
			if (state != MARIO_STATE_JUMP_HIGH)
				SetState(MARIO_STATE_JUMP_HIGH);
		}

		// if still holding S and pass fall point but not yet begin high jump point
		if (height > MARIO_JUMP_FALL_POINT && height < MARIO_BEGIN_HIGH_JUMP_HEIGHT) {
			if (keyboard->IsKeyDown(DIK_S)) {
				// if still holding S 
				vy = -MARIO_JUMP_PUSH - MARIO_GRAVITY * dt;
			}
			else {// fall ( because normal jump)
				vy = -MARIO_JUMP_PUSH / 2;
				SetState(MARIO_STATE_JUMP_FALL);
			}
		}

		// not high fall point yet
		if (state == MARIO_STATE_JUMP_HIGH && height < MARIO_HIGH_JUMP_FALL_POINT) {
			vy = -MARIO_JUMP_PUSH - MARIO_GRAVITY * dt;
		}
		else if (height > MARIO_HIGH_JUMP_FALL_POINT) {
			vy = -MARIO_JUMP_PUSH / 2;
			SetState(MARIO_STATE_JUMP_FALL);
		}
	}
	// still gets it but smoother  andddd  xử lý vụ đang rớt cái bấm S nữa, hơi sai
	
	
	DebugOut(L"STATE %d , height: %f, y: %f, vy: %f \n", state,height,y,vy);
}

void CMario::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects, 
	vector<LPCOLLISIONEVENT> coEvents, vector<LPCOLLISIONEVENT> &coEventsResult)
{
	coEvents.clear();
	
	//turn off collision when die
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
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

		SetIsOnGround(true);
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

	CGameObject::SetFlipOnNormal(nx);

		if (level == MARIO_LEVEL_BIG)
		{
		switch (this->state) {
			case MARIO_STATE_IDLE:
				ani = this->animations["Idle"];
				break;

			case MARIO_STATE_JUMP_IDLE:
				ani = this->animations["Idle"];
				break;

			case MARIO_STATE_WALK:
				if (GetSkid() != 0) {
					ani = this->animations["Skid"];
				}
				else {
					ani = this->animations["Walk"];
				}
				break;

			case MARIO_STATE_RUN:
				if (GetSkid() != 0) {
					ani = this->animations["Skid"];
				}
				else {
					ani = this->animations["HighSpeed"];
				}
				break;

			case MARIO_STATE_RUN_HIGH_SPEED:
				ani = this->animations["HighSpeed"];
				break;

			case MARIO_STATE_CROUCH:
				ani = this->animations["Crouch"];
				break;

			case MARIO_STATE_JUMP:
				ani = this->animations["Jump"];
				break;

			case MARIO_STATE_JUMP_HIGH:
				ani = this->animations["Jump"];
				break;

			case MARIO_STATE_JUMP_FALL:
				ani = this->animations["Fall"];
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

}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
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
		DebugOut(L"OnKeyUp ma dang state CROUCH nè, tới đây phải update lại bb với state \n");
		
		float currentX, currentY;
		GetPosition(currentX, currentY);

		SetPosition(currentX, currentY - MARIO_CROUCH_SUBSTRACT);
		SetSize(MARIO_BIG_BBOX_WIDTH, MARIO_BIG_BBOX_HEIGHT);

		SetState(MARIO_STATE_IDLE);

	}	

	if (keyCode == DIK_S) {
		if (GetIsOnGround() == true && state == MARIO_STATE_JUMP) {
			SetState(MARIO_STATE_IDLE);
		}
		else SetState(MARIO_STATE_JUMP);
	}
	
}
void CMario::OnKeyDown(int keyCode)
{
	switch (keyCode)
	{
		case DIK_DOWN: {
			float currentX, currentY;
			GetPosition(currentX, currentY);
			SetPosition(currentX, currentY + MARIO_CROUCH_SUBSTRACT);

			SetState(MARIO_STATE_CROUCH);
			SetFriction(MARIO_CROUCH_FRICTION);
		}
		break;

		case DIK_X: {
			if (state != MARIO_STATE_JUMP && state != MARIO_STATE_JUMP_FALL) {
				SetState(MARIO_STATE_JUMP);
				SetIsOnGround(false);
				GetPosY(jumpStartPosition);
				vy = -MARIO_JUMP_PUSH - MARIO_GRAVITY * dt;
			}
		}
		break;

		case DIK_S: {
			if (state != MARIO_STATE_JUMP && state!= MARIO_STATE_JUMP_FALL && GetIsOnGround()==true) {
				SetState(MARIO_STATE_JUMP);
				SetIsOnGround(false);
				GetPosY(jumpStartPosition);
				vy = - MARIO_JUMP_PUSH - MARIO_GRAVITY * dt;
				DebugOut(L"onKeyDown S nè\n");
			}
		}
		break;

		case DIK_A: {
			if (state == MARIO_STATE_WALK) {
				// prevent speed flickering when switching walk> run > walk .. everytime 
				SetState(MARIO_STATE_RUN);
				DebugOut(L" start run o onkeydown\n");
			}
		}
		break;

		case DIK_RIGHT: {
			finalKeyDirection = 1;
		}
		break;

		case DIK_LEFT: {
			finalKeyDirection = -1;
		}
		break;
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

void CMario::SetIsOnGround(bool onGround)
{
	this->isOnGround = onGround;
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
void CMario::ResetUntouchable()
{
	untouchable = 0;
	untouchable_start = 0;
}

void CMario::SetSkid(int skid)
{
	this->skid = skid;
}
int CMario::GetSkid()
{
	return this->skid;
}

void CMario::SetFriction(float friction)
{
	this->friction_x = friction;
}
int CMario::GetFriction()
{
	return this->friction_x;
}
