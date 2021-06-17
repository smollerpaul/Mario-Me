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
#include "FireBall.h"
#include "RedGoomba.h"

using namespace std;

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

	this->width = MARIO_BIG_BBOX_WIDTH;
	this->height = MARIO_BIG_BBOX_HEIGHT;

}

void CMario::InitAnimations()
{
	if (this->animations.size() < 1 ) {
		/*this->animations["Idle"] = CAnimations::GetInstance()->Get("ani-raccoon-mario-idle");
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

		this->animations["Spin"] = CAnimations::GetInstance()->Get("ani-raccoon-mario-spin");*/

		this->animations["Idle"] = CAnimations::GetInstance()->Get("ani-raccoon-mario-idle");
		this->animations["Walk"] = CAnimations::GetInstance()->Get("ani-raccoon-mario-walk");
		this->animations["Run"] = CAnimations::GetInstance()->Get("ani-raccoon-mario-walk");
		this->animations["HighSpeed"] = CAnimations::GetInstance()->Get("ani-raccoon-mario-speed-up");
		this->animations["Jump"] = CAnimations::GetInstance()->Get("ani-raccoon-mario-jump");
		this->animations["HighJump"] = CAnimations::GetInstance()->Get("ani-raccoon-mario-jump");
		this->animations["Fly"] = CAnimations::GetInstance()->Get("ani-raccoon-mario-fly");
		this->animations["Float"] = CAnimations::GetInstance()->Get("ani-raccoon-mario-float");
		this->animations["Fall"] = CAnimations::GetInstance()->Get("ani-raccoon-mario-fall");
		this->animations["Skid"] = CAnimations::GetInstance()->Get("ani-raccoon-mario-skid");
		this->animations["Crouch"] = CAnimations::GetInstance()->Get("ani-raccoon-mario-crouch");
		this->animations["Kick"] = CAnimations::GetInstance()->Get("ani-raccoon-mario-kick");
		

		this->animations["Hold"] = CAnimations::GetInstance()->Get("ani-raccoon-mario-hold");
		this->animations["HoldIdle"] = CAnimations::GetInstance()->Get("ani-raccoon-mario-hold-idle");
		this->animations["HoldFall"] = CAnimations::GetInstance()->Get("ani-raccoon-mario-hold-fall");

		this->animations["TeleVer"] = CAnimations::GetInstance()->Get("ani-raccoon-mario-idle-front");
		this->animations["TeleHor"] = CAnimations::GetInstance()->Get("ani-raccoon-mario-walk");
		this->animations["Spin"] = CAnimations::GetInstance()->Get("ani-raccoon-mario-spin");
		this->animations["AttackFire"] = CAnimations::GetInstance()->Get("ani-fire-mario-throw");

		/*this->animations["Idle"] = CAnimations::GetInstance()->Get("ani-fire-mario-idle");
		this->animations["Walk"] = CAnimations::GetInstance()->Get("ani-fire-mario-walk");
		this->animations["Run"] = CAnimations::GetInstance()->Get("ani-fire-mario-run");
		this->animations["HighSpeed"] = CAnimations::GetInstance()->Get("ani-fire-mario-high-speed");
		this->animations["Jump"] = CAnimations::GetInstance()->Get("ani-fire-mario-jump");
		this->animations["HighJump"] = CAnimations::GetInstance()->Get("ani-fire-mario-high-jump");
		this->animations["Fly"] = CAnimations::GetInstance()->Get("ani-fire-mario-high-jump");
		this->animations["Fall"] = CAnimations::GetInstance()->Get("ani-fire-mario-fall");
		this->animations["Float"] = CAnimations::GetInstance()->Get("ani-fire-mario-high-jump");
		this->animations["Skid"] = CAnimations::GetInstance()->Get("ani-fire-mario-skid");
		this->animations["Crouch"] = CAnimations::GetInstance()->Get("ani-fire-mario-crouch");
		this->animations["Kick"] = CAnimations::GetInstance()->Get("ani-fire-mario-kick");
		
		this->animations["Hold"] = CAnimations::GetInstance()->Get("ani-fire-mario-hold");
		this->animations["HoldIdle"] = CAnimations::GetInstance()->Get("ani-fire-mario-hold-idle");
		this->animations["HoldFall"] = CAnimations::GetInstance()->Get("ani-fire-mario-hold-fall");

		this->animations["TeleVer"] = CAnimations::GetInstance()->Get("ani-fire-mario-idle-front");
		this->animations["TeleHor"] = CAnimations::GetInstance()->Get("ani-fire-mario-walk");*/

		DebugOut(L"done init ani MARIO \n");
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
	RunPowerMeter(dt);
	JumpUpdate(dt);
	AttackUpdate(dt);
	CollisionUpdate(dt, coObjects, coEvents, coEventsResult);
	BehaviorUpdate(dt, coEventsResult);

	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];	


	//DebugOut(L" MARIO: vx: %f  vy: %f state: %d\n", vx, vy, state);
}

bool CMario::CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy)
{
	switch (obj->GetObjectType()) {
	case CGoomba::ObjectType:
		return coEventNx != 0;
		break;

	case RedGoomba::ObjectType:
		return coEventNx != 0 || coEventNy < 0;
		break;
	}
}

void CMario::MovementUpdate(DWORD dt)
{
	ResetFlip();
	Keyboard* keyboard = CGame::GetInstance()->GetKeyboard();
	CGameObject::Update(dt);

	// MOVE ON vx while flying tooo
	if (state == MARIO_STATE_FLY) {
		vx = MARIO_FLY_SPEED_X;
	}

#pragma region DOWN
	if (keyboard->IsKeyDown(DIK_DOWN)) {
		if (state != MARIO_STATE_WALK && state != MARIO_STATE_RUN && state != MARIO_STATE_CROUCH) {
			if (state == MARIO_STATE_ATTACK || state== MARIO_STATE_FIRE) {
				ResetAttackTimer();
			}
			SetState(MARIO_STATE_CROUCH);
		}
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

		if (GetIsOnGround()==true) {
			if (state != MARIO_STATE_WALK) {
				SetState(MARIO_STATE_WALK);
			}
		}

		float maxSpeed = MARIO_WALK_SPEED;
		accelerate_x = direction * MARIO_WALK_ACCELERATION;

		if (keyboard->IsKeyDown(DIK_A) && GetIsOnGround()== true) {
			// when fly -> dont change ani into ani RUN
			if (state != MARIO_STATE_RUN && state!= MARIO_STATE_FLY && state != MARIO_STATE_FLOAT) {
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

			if (GetIsOnGround() == false) {
				accelerate_x = MARIO_SKID_ACCELERATION * direction * 2;
			}
		}

		vx += accelerate_x * dt;

		// if RUN maxSpeed is reached -> upstep power meter
		if (abs(vx) >= maxSpeed) {
			vx = direction * maxSpeed;
			if (state == MARIO_STATE_RUN)
				isAtMaxRunSpeed = 1;
		} else isAtMaxRunSpeed = 0;

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
			if (state != MARIO_STATE_CROUCH && GetIsOnGround() == true && isAttacking ==0) {
				SetState(MARIO_STATE_IDLE);
			}
		}
	}

	// set friction
	if (state != MARIO_STATE_CROUCH) {
		if (state == MARIO_STATE_RUN) {
			SetFriction(MARIO_RUN_FRICTION);
		}
		else {
			SetFriction(MARIO_WALK_FRICTION);
		}
	}
	
	if (GetIsOnGround() == false) {
		SetFriction(0);
	}
#pragma endregion

}

void CMario::JumpUpdate(DWORD dt)
{
	Keyboard* keyboard = CGame::GetInstance()->GetKeyboard();

	float height = 0;

	// stop float , cannot fly anymore when drops to the ground
	if (GetIsOnGround() == 1 ) {
		ResetFloatTimer();
	}

	// if falls to ground -> set state idle
	if (state == MARIO_STATE_JUMP_FALL) {
		if (GetIsOnGround() == true) {
			SetState(MARIO_STATE_IDLE);
			vy = MARIO_GRAVITY * dt;
		}
	}

	//calc height
	if (state == MARIO_STATE_JUMP || state == MARIO_STATE_JUMP_HIGH || state == MARIO_STATE_JUMP_FALL || state == MARIO_STATE_FLY || state == MARIO_STATE_FLOAT) {
		height = abs(jumpStartPosition - y - vy * dt);
	}

	//bam S once n activate fly

	// fly for a time -> float
	if (state == MARIO_STATE_FLY) {
		if (height >= MARIO_FLY_MAX_POINT) {
			SetState(MARIO_STATE_FLOAT);
			vy = -MARIO_FLY_PUSH / 2;
		} else vy = -MARIO_FLY_PUSH - MARIO_GRAVITY * dt;
	}

	// float down
	if (state == MARIO_STATE_FLOAT) {
		// if float over 500ms -> cannot press S to fly anymore
		floatTimer += dt;
		vy = MARIO_FLY_PUSH / 2;

		if (floatTimer < MARIO_FLOAT_TIME) {
			if (keyboard->IsKeyDown(DIK_S)) {
				ResetFloatTimer();

				if (height >= MARIO_FLY_MAX_POINT) {
					DebugOut(L" you cannot press S anymoreeee! \n");
					floatTimer = MARIO_FLOAT_TIME; // SO i cannot press s anymore
				}
				else {
					SetState(MARIO_STATE_FLY);
				}
			}
		}
	}

#pragma region JUMP & HIGH_JUMP & FALL
	if (state != MARIO_STATE_JUMP_FALL && state!= MARIO_STATE_FLY && state != MARIO_STATE_FLOAT) {
		// continue jump when  ( not reaching fall point yet) HIGH & LOW
		if ((height < MARIO_JUMP_FALL_POINT && height != 0)
			|| (state == MARIO_STATE_JUMP_HIGH && height < MARIO_HIGH_JUMP_FALL_POINT)) {
			vy = -MARIO_JUMP_PUSH - MARIO_GRAVITY * dt;
		}

		//set state HIGH
		if (height > MARIO_BEGIN_HIGH_JUMP_HEIGHT && state == MARIO_STATE_JUMP) {
			if (state != MARIO_STATE_JUMP_HIGH)
				SetState(MARIO_STATE_JUMP_HIGH);
		}

		// if still holding S + pass fall point + not yet HIGH
		if (height > MARIO_JUMP_FALL_POINT && height < MARIO_BEGIN_HIGH_JUMP_HEIGHT) {
			if (keyboard->IsKeyDown(DIK_S)) {
				vy = -MARIO_JUMP_PUSH - MARIO_GRAVITY * dt;
			}
			else { //fall LOW
				vy = -MARIO_JUMP_PUSH / 2;
				SetState(MARIO_STATE_JUMP_FALL);
			}
		}

		//fall HIGH
		if (height > MARIO_HIGH_JUMP_FALL_POINT && state == MARIO_STATE_JUMP_HIGH) {
			vy = -MARIO_JUMP_PUSH / 2;
			SetState(MARIO_STATE_JUMP_FALL);
		}
	}
#pragma endregion
}

void CMario::AttackUpdate(DWORD dt)
{
	Keyboard* keyboard = CGame::GetInstance()->GetKeyboard();
	if (isAttacking == 1) {
		if (state != MARIO_STATE_ATTACK) {
			SetState(MARIO_STATE_ATTACK);
		}

		float currentTime = dt;
		attackTimer += currentTime;

		DebugOut(L" ATTACK TIMER: %f \n", attackTimer);

		if (attackTimer >= MARIO_ATTACK_TIME) {
			isAttacking = 0;
			ResetAttackTimer();
		}
	}
	else if (isAttackingFire == 1) {
		if (state != MARIO_STATE_FIRE) {
			SetState(MARIO_STATE_FIRE);
		}

		float currentTime = dt;
		attackTimer += currentTime;

		DebugOut(L" FIRE TIMER: %f \n", attackTimer);

		if (attackTimer >= MARIO_FIRE_TIME) {
			isAttackingFire = 0;
			ResetAttackTimer();
		}
	}
}

void CMario::RunPowerMeter(DWORD dt)
{
	//power meter increase as long as onground , mario is at maxspeed
	if (GetIsOnGround() == true && isAtMaxRunSpeed == 1 ) {
			powerMeter = min(powerMeter + PM_INCREASE * dt, PM_MAX);
	}
	else { 
		//powerMeter can't be <0
		powerMeter = max(powerMeter - PM_DECREASE * dt, 0.0f);
	}
}

void CMario::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT> coEvents, vector<LPCOLLISIONEVENT>& coEventsResult)
{
	coEvents.clear();

	//turn off collision when die
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		CGameObject::UpdatePosition();
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0; //predefined stuff , value doesnt matter

		float rdx = 0;
		float rdy = 0;

		//is get clamp distance , still got those coEvents
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		// if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		// updated position

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
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
					else if (e->nx != 0)
					{
						if (untouchable == 0)
						{
							if (level != MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				}
				break;

			case RedGoomba::ObjectType:
			{
				RedGoomba* rg = dynamic_cast<RedGoomba*>(e->obj);

				if (e->ny < 0)
				{
					vy = -MARIO_JUMP_DEFLECT_SPEED;
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (level != MARIO_LEVEL_SMALL)
						{
							level = MARIO_LEVEL_SMALL;
							StartUntouchable();
						}
						else
							SetState(MARIO_STATE_DIE);
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

	switch (this->state) {
	case MARIO_STATE_IDLE: 
		if (vy != 0) {
			ani = this->animations["Fall"];
		} else
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

	case MARIO_STATE_FLY:
		ani = this->animations["Fly"];
		break;

	case MARIO_STATE_FLOAT:
		ani = this->animations["Float"];
		break;

	case MARIO_STATE_ATTACK:
		ani = this->animations["Spin"];
		break;

	case MARIO_STATE_FIRE:
		ani = this->animations["AttackFire"];
		break;
	}

	int alpha = 255;
	if (untouchable)
		alpha = 128;

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	float l, t, b, r;
	GetBoundingBox(l, t, r, b);

	ani->SetPlayScale(1.5f);
	ani->Render(x - camera->GetX() + (r - l) / 2, y - camera->GetY() + (b - t) / 2, flip, alpha);

	RenderBoundingBox();
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
int CMario::GetObjectType()
{
	return ObjectType;
}

void CMario::OnKeyUp(int keyCode)
{
	//when not DIK left right -> not maxSpeed for run to trigger power meter duh
	isAtMaxRunSpeed = 0;

	if (state == MARIO_STATE_CROUCH) {
		float currentX, currentY;
		GetPosition(currentX, currentY);

		SetPosition(currentX, currentY - MARIO_CROUCH_SUBSTRACT);
		SetSize(MARIO_BIG_BBOX_WIDTH, MARIO_BIG_BBOX_HEIGHT);

		SetState(MARIO_STATE_IDLE);
	}	

	if (keyCode == DIK_S) {
		// from fly to float when release S
		if (state == MARIO_STATE_FLY) {
			SetState(MARIO_STATE_FLOAT);
			DebugOut(L" float when keyup S\n");
		}
		// fall even when high jumping
		else  if (GetIsOnGround() == true ) {
			SetState(MARIO_STATE_IDLE);
		}

		// float down when S released but keep floating ani
		else if (state != MARIO_STATE_FLOAT) {
			SetState(MARIO_STATE_JUMP_FALL);
		}
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
			if (GetIsOnGround() == true) {
				if (powerMeter >= PM_MAX) {
					SetState(MARIO_STATE_FLY);
					vy = -MARIO_FLY_PUSH * 3 - MARIO_GRAVITY * dt;
				} //always go here
				else {
					SetState(MARIO_STATE_JUMP);
					vy = -MARIO_JUMP_PUSH - MARIO_GRAVITY * dt;
				}
				SetIsOnGround(false);
				GetPosY(jumpStartPosition);
			}
			else {
				if (state == MARIO_STATE_FLOAT ) {
					vy -= MARIO_JUMP_PUSH/2 ;
				}

				if (state == MARIO_STATE_FLY) {
					vy = -MARIO_FLY_PUSH - MARIO_GRAVITY * dt;
					DebugOut(L" fly up when press S \n");
				}
			}
		}
		break;

		case DIK_A: {
			if (state == MARIO_STATE_WALK) {
				SetState(MARIO_STATE_RUN);
			}
			else {
				isAttacking = 1;
				DebugOut(L"A pressed \n");
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

		case DIK_F: {
			FireBall* fb = new FireBall(this);
			CGame::GetInstance()->GetCurrentScene()->AddObject(fb);

			isAttackingFire = 1;
			DebugOut(L"ball\n");
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

void CMario::ResetAttackTimer()
{
	attackTimer = 0;
	DebugOut(L"Attack timer resetted!\n");
}

void CMario::SetIsOnGround(bool onGround)
{
	this->isOnGround = onGround;
}
bool CMario::GetIsOnGround()
{
	return isOnGround;
}

void CMario::SetLevel(int l)
{
	this->level = l;
}

void CMario::StartUntouchable()
{
	DebugOut(L" start untouchable\n");
	untouchable = 1;
	untouchable_start = GetTickCount64();
}
void CMario::ResetUntouchable()
{
	untouchable = 0;
	untouchable_start = 0;
}

void CMario::ResetFloatTimer()
{
	floatTimer = 0;
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
