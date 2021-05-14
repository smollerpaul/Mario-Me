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

	if (state == MARIO_STATE_JUMP) {
		if (GetIsOnGround()) {
			SetState(MARIO_STATE_IDLE);
		}

		if (keyboard->IsKeyDown(DIK_S)) {

		}
	}

	/*	dang run, walk -> cant crouch
		dang idle -> can crouch
		dang crouch -> can run, walk
	*/

	if (keyboard->IsKeyDown(DIK_DOWN)) {
		if (state != MARIO_STATE_WALK && state != MARIO_STATE_RUN && state != MARIO_STATE_CROUCH)
			SetState(MARIO_STATE_CROUCH);

		if (state == MARIO_STATE_CROUCH) {
			if (vx != 0)
				vx = 0;

			SetSize(MARIO_BIG_BBOX_WIDTH, MARIO_BIG_BBOX_HEIGHT - MARIO_CROUCH_SUBSTRACT);
		}
	}

	if (keyboard->IsKeyDown(DIK_RIGHT) || keyboard->IsKeyDown(DIK_LEFT)) {
		int direction = 0;

		direction = lastKeyDirection;

		//get up from crouch to walk
		if (state == MARIO_STATE_CROUCH) {
			float currentX, currentY;
			GetPosition(currentX, currentY);

			SetPosition(currentX, currentY - MARIO_CROUCH_SUBSTRACT);
			SetSize(MARIO_BIG_BBOX_WIDTH, MARIO_BIG_BBOX_HEIGHT);
		}

		if (isOnGround) {
			if (state != MARIO_STATE_WALK)
				SetState(MARIO_STATE_WALK);
		}

		float maxSpeed = MARIO_WALK_SPEED;
		accelerate_x = direction * MARIO_WALK_ACCELERATION;

		if (keyboard->IsKeyDown((DIK_A))) {
			if (state != MARIO_STATE_RUN) {
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

		
		if (abs(vx) >= maxSpeed) {
			vx = direction * maxSpeed;
		}
		
		if (vx * direction >= 0) {
			SetSkid(0);
		}

		SetDirection(direction);
	}

	/*if (GetSkid() == 1) {
		if (abs(vx) >= maxSpeed) {
			vx = direction * maxSpeed;
		}
		else {
			float dragForce;
			int dragForceDir = (vx > 0) ? -1 : 1;

			dragForce = dragForceDir * MARIO_WALK_DRAG_FORCE;

			if (keyboard->IsKeyDown(DIK_A)) {
				dragForce = dragForceDir * MARIO_RUN_DRAG_FORCE;
			}
			vx -= dt * dragForce;
		}
	}*/


#pragma region JUMP

	//if (GetIsOnGround() == false && state == MARIO_STATE_JUMP) {
	//	vy -= MARIO_JUMP_PUSH * dt;

	//	if (this->y == MARIO_MIN_JUMP_HEIGHT) {
	//		//neu mario height dung min -> cho no max
	//		if (keyboard->IsKeyDown(DIK_S)) {
	//			if (state != MARIO_STATE_JUMP_HIGH)
	//				SetState(MARIO_STATE_JUMP_HIGH);

	//			//set dieu kien de no flinch a bit

	//			vy += (MARIO_JUMP_PUSH * dt)/10;

	//			//neu mario height dung max -> cho no fall, khi fall
	//			if (this->y == MARIO_JUMP_HEIGHT) {
	//				DebugOut(L"FALLLLLLLLLLLLLLLLLLLLLLLLLLLL\n");

	//				SetState(MARIO_STATE_JUMP_FALL);


	// co giu S, cham dat -> idle ok
	// du co giu S, khi cham dat van -> idle
	
#pragma endregion

	DebugOut(L"[state]: %d , [face]: %d \n", state,nx);
	//DebugOut(L"[state]: %d  [vy]: %f   [y]: %f   \n", state, vy,y);
	//DebugOut(L"[MARIO SPEED]:  vx: %f, SKID %d, STATE %d \n", vx, skid,state );
	//(L"[MARIO SPEED]:  posX: %f, posY: %f, vx: %f, vy: %f\n", x, y, vx, vy);
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


void CMario::SetSkid(int skid)
{
	this->skid = skid;
}

int CMario::GetSkid()
{
	return this->skid;
}


void CMario::OnKeyUp(int keyCode)
{
	if (state == MARIO_STATE_CROUCH) {
		float currentX, currentY;
		GetPosition(currentX, currentY);

		SetPosition(currentX, currentY -  MARIO_CROUCH_SUBSTRACT);
		SetSize(MARIO_BIG_BBOX_WIDTH, MARIO_BIG_BBOX_HEIGHT);

	}	

	if (GetIsOnGround() == true && state!=MARIO_STATE_IDLE) {
		SetState(MARIO_STATE_IDLE);
	}
}

void CMario::OnKeyDown(int keyCode)
{

	// xet both cases so they will have a direction to adhere to
	if (keyCode == DIK_RIGHT) {
		lastKeyDirection = 1;
	}
	else if (keyCode == DIK_LEFT) {
		lastKeyDirection = -1;
	}

	DebugOut(L"last key direct in OnKeyDown(): %d  \n", lastKeyDirection);
	
	switch (keyCode)
	{
		case DIK_SPACE: {
			SetState(MARIO_STATE_JUMP);
		}
		break;

		case DIK_DOWN: {
			float currentX, currentY;
			GetPosition(currentX, currentY);
			SetPosition(currentX, currentY + MARIO_CROUCH_SUBSTRACT);

			SetState(MARIO_STATE_CROUCH);
		}
		break;

		case DIK_X: {
			if (state != MARIO_STATE_JUMP) {
				SetState(MARIO_STATE_JUMP);
				SetIsOnGround(false);
				vy = -MARIO_JUMP_PUSH * dt;
			}
		}
		break;

		case DIK_S: {
			if (state != MARIO_STATE_JUMP) {
				SetState(MARIO_STATE_JUMP);
				SetIsOnGround(false);
				vy = -MARIO_JUMP_PUSH * dt;
				DebugOut(L" S pressed\n");
			}
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


