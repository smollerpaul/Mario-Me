#include "FireMario.h"

FireMario::FireMario() {
}

FireMario::FireMario(CMario* masterObj) {
	this->master = masterObj;

	if (master->height != 81) {
		master->SetPosition(master->x, master->y - 40);
		DebugOut(L"x: %f, y:%f, height: %f\n", master->x, master->y, master->height);
	}

	master->SetSize(MARIO_WIDTH, MARIO_HEIGHT);
}

void FireMario::InitAnimations()
{
	if (this->animations.size() < 1) {
		this->animations["AttackFire"] = CAnimations::GetInstance()->Get("ani-fire-mario-throw")->Clone();
		this->animations["Idle"] = CAnimations::GetInstance()->Get("ani-fire-mario-idle")->Clone();
		this->animations["Walk"] = CAnimations::GetInstance()->Get("ani-fire-mario-walk")->Clone();
		this->animations["Run"] = CAnimations::GetInstance()->Get("ani-fire-mario-run")->Clone();
		this->animations["HighSpeed"] = CAnimations::GetInstance()->Get("ani-fire-mario-high-speed")->Clone();
		this->animations["Jump"] = CAnimations::GetInstance()->Get("ani-fire-mario-jump")->Clone();
		this->animations["HighJump"] = CAnimations::GetInstance()->Get("ani-fire-mario-high-jump")->Clone();
		this->animations["Fly"] = CAnimations::GetInstance()->Get("ani-fire-mario-high-jump")->Clone();
		this->animations["Fall"] = CAnimations::GetInstance()->Get("ani-fire-mario-fall")->Clone();
		this->animations["Float"] = CAnimations::GetInstance()->Get("ani-fire-mario-high-jump")->Clone();
		this->animations["Skid"] = CAnimations::GetInstance()->Get("ani-fire-mario-skid")->Clone();
		this->animations["Crouch"] = CAnimations::GetInstance()->Get("ani-fire-mario-crouch")->Clone();
		this->animations["Kick"] = CAnimations::GetInstance()->Get("ani-fire-mario-kick")->Clone();

		this->animations["Hold"] = CAnimations::GetInstance()->Get("ani-fire-mario-hold")->Clone();
		this->animations["HoldIdle"] = CAnimations::GetInstance()->Get("ani-fire-mario-hold-idle")->Clone();
		this->animations["HoldFall"] = CAnimations::GetInstance()->Get("ani-fire-mario-hold-fall")->Clone();

		this->animations["TeleVer"] = CAnimations::GetInstance()->Get("ani-fire-mario-idle-front")->Clone();
		this->animations["TeleHor"] = CAnimations::GetInstance()->Get("ani-fire-mario-walk")->Clone();

		this->animations["Poof"] = CAnimations::GetInstance()->Get("ani-fireball-damaged")->Clone();

	}
}

void FireMario::Update(DWORD dt)
{
	if (master->untouchable == 1) {
		master->untouchableTimer += dt;

		if (master->untouchableTimer >= MARIO_UNTOUCHABLE_TIME) {
			master->ResetUntouchable();

			if (powerUpLeaf == 1) {
				master->SetObjectState(new RacoonMario(master));
				powerUpLeaf = 0;
			} 
			else {
				if (powerUpMushroom != 0)
					powerUpMushroom = 0;
				master->SetObjectState(new BigMario(master));
			}

			master->visible = 1;
		}
	}
}

bool FireMario::CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy)
{
	return master->untouchable = 1;
}

void FireMario::MovementUpdate(DWORD dt)
{
	if (master->untouchable != 1) {
		master->ResetFlip();
		Keyboard* keyboard = CGame::GetInstance()->GetKeyboard();

		master->dt = dt;
		master->dx = master->vx * dt;
		master->dy = master->vy * dt;

		// MOVE ON vx while flying tooo
		if (master->state == MARIO_STATE_FLY) {
			master->vx = master->nx * MARIO_FLY_SPEED_X;
		}

#pragma region DOWN
		if (keyboard->IsKeyDown(DIK_DOWN)) {
			if (master->state != MARIO_STATE_WALK && master->state != MARIO_STATE_RUN && master->state != MARIO_STATE_CROUCH) {
				if (master->state == MARIO_STATE_ATTACK) {
					master->ResetAttackTimer();
				}
				master->SetState(MARIO_STATE_CROUCH);
			}
			if (master->state == MARIO_STATE_CROUCH) {
				if (master->vx != 0)
					master->vx = 0;

				master->SetSize(MARIO_WIDTH, MARIO_HEIGHT - MARIO_CROUCH_SUBSTRACT);
			}
		}
#pragma endregion

#pragma region RIGHT LEFT
		if (keyboard->IsKeyDown(DIK_RIGHT) || keyboard->IsKeyDown(DIK_LEFT)) {
			int direction = 0;
			direction = master->finalKeyDirection;

			//get up from crouch to walk
			if (master->state == MARIO_STATE_CROUCH) {
				float currentX, currentY;
				master->GetPosition(currentX, currentY);

				master->SetPosition(currentX, currentY - MARIO_CROUCH_SUBSTRACT);
				master->SetSize(MARIO_WIDTH, MARIO_HEIGHT);
			}

			if (master->isOnGround == true) {
				if (master->state != MARIO_STATE_WALK) {
					master->SetState(MARIO_STATE_WALK);
				}
			}

			float maxSpeed = MARIO_WALK_SPEED;
			master->accelerate_x = direction * MARIO_WALK_ACCELERATION;

			if (keyboard->IsKeyDown(DIK_A) && master->isOnGround == true) {
				// when fly -> dont change ani into ani RUN
				if (master->state != MARIO_STATE_RUN && master->state != MARIO_STATE_FLY && master->state != MARIO_STATE_FLOAT) {
					master->SetState(MARIO_STATE_RUN);
				}
				maxSpeed = MARIO_RUN_SPEED;
				master->accelerate_x = direction * MARIO_RUN_ACCELERATION;
			}

			//skid when direction is against vx
			if (master->vx * direction < 0 && master->isOnGround != 0) {
				master->SetSkid(1);
				master->accelerate_x = direction * MARIO_SKID_ACCELERATION;

				if (keyboard->IsKeyDown(DIK_A)) {
					master->accelerate_x = 2 * direction * MARIO_SKID_ACCELERATION;
				}

				if (master->isOnGround == false) {
					master->accelerate_x = MARIO_SKID_ACCELERATION * direction * 2;
				}
			}

			master->vx += master->accelerate_x * dt;

			// if RUN maxSpeed is reached -> upstep power meter
			if (abs(master->vx) >= maxSpeed) {
				master->vx = direction * maxSpeed;
				if (master->state == MARIO_STATE_RUN)
					master->isAtMaxRunSpeed = 1;
			}
			else master->isAtMaxRunSpeed = 0;

			if (master->vx * direction >= 0) {
				master->SetSkid(0);
			}

			master->SetDirection(direction);
		}
		else {
			//still goes in here everytime
			master->SetSkid(0);

			// slow down to reach vx=0 (stop)
			if (abs(master->vx) > MARIO_WALK_FRICTION * dt) {
				int speedDirection = (master->vx > 0) ? 1 : -1;
				master->vx -= speedDirection * MARIO_WALK_FRICTION * dt;
			}
			else {
				master->vx = 0;
				if (master->state != MARIO_STATE_CROUCH && master->isOnGround == true && master->isAttacking == 0) {
					master->SetState(MARIO_STATE_IDLE);
				}
			}
		}

		// set friction
		if (master->state != MARIO_STATE_CROUCH) {
			if (master->state == MARIO_STATE_RUN) {
				master->SetFriction(MARIO_RUN_FRICTION);
			}
			else {
				master->SetFriction(MARIO_WALK_FRICTION);
			}
		}

		if (master->isOnGround == false) {
			master->SetFriction(0);
		}
#pragma endregion
	}	
}

void FireMario::JumpUpdate(DWORD dt)
{
	SmallMario::JumpUpdate(dt);
}

void FireMario::AttackUpdate(DWORD dt)
{
	if (master->untouchable != 1) {
		Keyboard* keyboard = CGame::GetInstance()->GetKeyboard();
		if (master->isAttacking == 1) {
			if (master->state != MARIO_STATE_ATTACK) {
				master->SetState(MARIO_STATE_ATTACK);
			}
			float currentTime = dt;
			master->attackTimer += currentTime;
			if (master->attackTimer >= MARIO_ATTACK_TIME_FIRE) {
				master->isAttacking = 0;
				master->ResetAttackTimer();
			}
		}
	}
}

void FireMario::BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult, vector<LPCOLLISIONEVENT> coEvents)
{
	PlayerData* pd = PlayerData::GetInstance();

	if (master->untouchable != 1) {
		SmallMario::PostCollisionUpdate(dt, coEventsResult, coEvents);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			switch (e->obj->GetObjectType()) {

			case CGoomba::ObjectType:
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				if (e->ny < 0) {
					master->vy = -MARIO_JUMP_DEFLECT_SPEED;
					pd->SetScore(pd->GetScore() + 100);
				}

				if (e->nx != 0) {
					if (master->untouchable == 0) {
						master->StartUntouchable();
						master->visible = 0;
					}
					EffectVault::GetInstance()->AddEffect(new MarioTransform(master->x, master->y + 25, MARIO_UNTOUCHABLE_TIME));
				}
			}
			break;

			case RedGoomba::ObjectType:
			{
				RedGoomba* rg = dynamic_cast<RedGoomba*>(e->obj);

				if (e->ny < 0) {
					master->vy = -MARIO_JUMP_DEFLECT_SPEED;
					pd->SetScore(pd->GetScore() + 100);
				}
				else if (e->nx != 0) {
					if (master->untouchable == 0) {
						master->StartUntouchable();
						master->visible = 0;
					}
					EffectVault::GetInstance()->AddEffect(new MarioTransform(master->x, master->y + 25, MARIO_UNTOUCHABLE_TIME));
				}
			}
			break;

			case NormalKoopas::ObjectType:
			{
				NormalKoopas* rg = dynamic_cast<NormalKoopas*>(e->obj);

				if (e->ny < 0) {
					master->vy = -MARIO_JUMP_DEFLECT_SPEED;
					pd->SetScore(pd->GetScore() + 100);
				}
				else if (e->nx != 0 || e->ny > 0) {
					if (master->untouchable == 0) {
						master->StartUntouchable();
						master->visible = 0;
					}
					EffectVault::GetInstance()->AddEffect(new MarioTransform(master->x, master->y + 25, MARIO_UNTOUCHABLE_TIME));
				}
			}
			break;

			case SlidingShell::ObjectType:
			{
				SlidingShell* rg = dynamic_cast<SlidingShell*>(e->obj);

				if (e->ny < 0) {
					master->vy = -MARIO_JUMP_DEFLECT_SPEED;
				}
				else if (e->nx != 0 || e->ny > 0) {
					if (master->untouchable == 0) {
						master->StartUntouchable();
						master->visible = 0;
					}
					EffectVault::GetInstance()->AddEffect(new MarioTransform(master->x, master->y + 25, MARIO_UNTOUCHABLE_TIME));
				}
			}
			break;

			case WingedKoopas::ObjectType:
			{
				WingedKoopas* rg = dynamic_cast<WingedKoopas*>(e->obj);

				if (e->ny < 0) {
					master->vy = -MARIO_JUMP_DEFLECT_SPEED;
					pd->SetScore(pd->GetScore() + 100);
				}
				else if (e->nx != 0 || e->ny > 0) {
					if (master->untouchable == 0) {
						master->StartUntouchable();
						master->visible = 0;
					}
					EffectVault::GetInstance()->AddEffect(new MarioTransform(master->x, master->y + 25, MARIO_UNTOUCHABLE_TIME));
				}
			}
			break;

			case ShelledKoopas::ObjectType:
			{
				ShelledKoopas* rg = dynamic_cast<ShelledKoopas*>(e->obj);

				if (e->ny < 0) {
					master->vy = -MARIO_JUMP_DEFLECT_SPEED;
				}
			}
			break;

			case FireBall::ObjectType:
			{
				FireBall* fb = dynamic_cast<FireBall*>(e->obj);

				if (e->ny < 0) {
					master->vy = -MARIO_JUMP_DEFLECT_SPEED;
				}

				if (e->nx != 0) {
					if (master->untouchable == 0) {
						master->StartUntouchable();
						master->visible = 0;
					}
					EffectVault::GetInstance()->AddEffect(new MarioTransform(master->x, master->y + 25, MARIO_UNTOUCHABLE_TIME));
				}
			}
			break;

			case Leaf::ObjectType:
			{
				Leaf* leaf = dynamic_cast<Leaf*>(e->obj);

				if (e->nx != 0 || e->ny != 0) {
					pd->SetScore(pd->GetScore() + 100);
					master->StartUntouchable();
					master->visible = 0;
					powerUpLeaf = 1;
					EffectVault::GetInstance()->AddEffect(new MarioTransform(master->x, master->y + 25, MARIO_UNTOUCHABLE_TIME));
				}
			}
			break;

			case EndCard::ObjectType:
			{
				EndCard* p = dynamic_cast<EndCard*>(e->obj);
				EffectVault::GetInstance()->AddEffect(new FlyingCard(8038, 973));

				p->SetAlive(0);
			}
			break;

			case Void::ObjectType:
			{
				Void* p = dynamic_cast<Void*>(e->obj);
				if (master->state != MARIO_STATE_DIE)
					master->SetState(MARIO_STATE_DIE);
				EffectVault::GetInstance()->AddEffect(new MarioDieFx(master->x, master->y));
			}
			break;

			}
		}
	}
}

void FireMario::Render()
{
	InitAnimations();
	CAnimation* ani = this->animations["Idle"];

	if (master->visible == 0)
		return;

	switch (master->state) {
		case MARIO_STATE_IDLE: 
			if (master->vy != 0)
				ani = this->animations["Fall"];
			else
				ani = this->animations["Idle"];
	
		break;

		case MARIO_STATE_JUMP_IDLE:
			ani = this->animations["Idle"];
			break;

		case MARIO_STATE_WALK:
			if (master->skid != 0) {
				ani = this->animations["Skid"];
			}
			else {
				ani = this->animations["Walk"];
			}
			break;

		case MARIO_STATE_RUN:
			if (master->skid != 0) {
				ani = this->animations["Skid"];
			}
			else {
				if (master->powerMeter >= PM_MAX) {
					ani = this->animations["HighSpeed"];
				}
				else
					ani = this->animations["Walk"];
			}
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
			ani = this->animations["AttackFire"];
			break;

		case MARIO_STATE_SHRINK:
			ani = this->animations["Poof"];
			break;
	}

	int alpha = 255;
	if (master->untouchable)
		alpha = 128;

	master->SetFlipOnNormal(master->nx);
	int flip = master->flip;

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	float l, t, b, r;
	master->GetBoundingBox(l, t, r, b);

	ani->SetPlayScale(1.5f);
	ani->Render(master->x - camera->GetX() + (r - l) / 2, master->y - camera->GetY() + (b - t) / 2, flip, alpha);

}

void FireMario::OnKeyUp(int keyCode)
{
	//when not DIK left right -> not maxSpeed for run to trigger power meter duh
	master->isAtMaxRunSpeed = 0;

	if (master->state == MARIO_STATE_CROUCH) {
		float currentX, currentY;
		master->GetPosition(currentX, currentY);

		master->SetPosition(currentX, currentY - MARIO_CROUCH_SUBSTRACT);
		master->SetSize(MARIO_WIDTH, MARIO_HEIGHT);

		master->SetState(MARIO_STATE_IDLE);
	}

	if (keyCode == DIK_S) {
		// from fly to float when release S
		if (master->state == MARIO_STATE_FLY) {
			master->SetState(MARIO_STATE_FLOAT);
			DebugOut(L" float when keyup S\n");
		}
		// fall even when high jumping
		else  if (master->isOnGround == true) {
			master->SetState(MARIO_STATE_IDLE);
		}

		// float down when S released but keep floating ani
		else if (master->state != MARIO_STATE_FLOAT) {
			master->SetState(MARIO_STATE_JUMP_FALL);
		}
	}
}

void FireMario::OnKeyDown(int keyCode)
{
	if (master->untouchable != 1) {
		switch (keyCode)
		{
		case DIK_DOWN: {
			float currentX, currentY;
			master->GetPosition(currentX, currentY);
			master->SetPosition(currentX, currentY + MARIO_CROUCH_SUBSTRACT);

			master->SetState(MARIO_STATE_CROUCH);
			master->SetFriction(MARIO_CROUCH_FRICTION);
		}
					 break;

		case DIK_X: {
			if (master->state != MARIO_STATE_JUMP && master->state != MARIO_STATE_JUMP_FALL) {
				master->SetState(MARIO_STATE_JUMP);
				master->SetIsOnGround(false);
				master->GetPosY(master->jumpStartPosition);
				master->vy = -MARIO_JUMP_PUSH - MARIO_GRAVITY * master->dt;
			}
		}
				  break;

		case DIK_S: {
			if (master->isOnGround == true) {
				if (master->powerMeter >= PM_MAX) {
					master->SetState(MARIO_STATE_FLY);
					master->vy = -MARIO_FLY_PUSH * 3 - MARIO_GRAVITY * master->dt;
				} //always go here
				else {
					master->SetState(MARIO_STATE_JUMP);
					master->vy = -MARIO_JUMP_PUSH - MARIO_GRAVITY * master->dt;
				}
				master->SetIsOnGround(false);
				master->GetPosY(master->jumpStartPosition);
			}
			else {
				if (master->state == MARIO_STATE_FLOAT) {
					master->vy -= MARIO_JUMP_PUSH / 2;
				}

				if (master->state == MARIO_STATE_FLY) {
					master->vy = -MARIO_FLY_PUSH - MARIO_GRAVITY * master->dt;
					//DebugOut(L" fly up when press S \n");
				}
			}
		}
				  break;

		case DIK_A: {
			if (master->state == MARIO_STATE_WALK) {
				master->SetState(MARIO_STATE_RUN);
			}
			else {
				FireBall* fb = new FireBall(master);
				CGame::GetInstance()->GetCurrentScene()->AddObject(fb);
				master->isAttacking = 1;
			}
		}
				  break;

		case DIK_RIGHT: {
			master->finalKeyDirection = 1;
		}
					  break;

		case DIK_LEFT: {
			master->finalKeyDirection = -1;
		}
					 break;

		case DIK_R: {
			master->Reset();
		}
				  break;
		}
	}
}
