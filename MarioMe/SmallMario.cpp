#include "SmallMario.h"
#include "Mario.h"
#include "GameObject.h"

SmallMario::SmallMario()
{
}

SmallMario::SmallMario(CMario* masterObj)
{
	this->master = masterObj;
	master->SetSize(MARIO_WIDTH, MARIO_WIDTH);

	master->y += 30;
}

void SmallMario::InitAnimations()
{
	if (this->animations.size() < 1 ) {
		this->animations["Idle"] = CAnimations::GetInstance()->Get("ani-small-mario-idle")->Clone();
		this->animations["Walk"] = CAnimations::GetInstance()->Get("ani-small-mario-walk")->Clone();
		this->animations["Run"] = CAnimations::GetInstance()->Get("ani-small-mario-run")->Clone();
		this->animations["HighSpeed"] = CAnimations::GetInstance()->Get("ani-small-mario-run")->Clone();
		this->animations["Jump"] = CAnimations::GetInstance()->Get("ani-small-mario-jump")->Clone();
		this->animations["Fly"] = CAnimations::GetInstance()->Get("ani-small-mario-jump")->Clone();
		this->animations["Fall"] = CAnimations::GetInstance()->Get("ani-small-mario-jump")->Clone();
		this->animations["Float"] = CAnimations::GetInstance()->Get("ani-small-mario-jump")->Clone();
		this->animations["Skid"] = CAnimations::GetInstance()->Get("ani-small-mario-skid")->Clone();
		this->animations["Kick"] = CAnimations::GetInstance()->Get("ani-small-mario-kick")->Clone();
		this->animations["Crouch"] = CAnimations::GetInstance()->Get("ani-small-mario-idle")->Clone();

		this->animations["Hold"] = CAnimations::GetInstance()->Get("ani-small-mario-hold")->Clone();
		this->animations["HoldIdle"] = CAnimations::GetInstance()->Get("ani-small-mario-hold-idle")->Clone();
		this->animations["HoldFall"] = CAnimations::GetInstance()->Get("ani-small-mario-hold-fall")->Clone();

		this->animations["TeleVer"] = CAnimations::GetInstance()->Get("ani-small-mario-idle-front")->Clone();
		this->animations["TeleHor"] = CAnimations::GetInstance()->Get("ani-small-mario-walk")->Clone();
		this->animations["Grow"] = CAnimations::GetInstance()->Get("ani-mario-grow-up")->Clone();

		this->animations["Die"] = CAnimations::GetInstance()->Get("ani-small-mario-die")->Clone();
	}
}

void SmallMario::Update(DWORD dt)
{
	PlayerData* pd = PlayerData::GetInstance();

	//tele
	if (teleporting == 1) {
		teleportHold += dt;
		master->vx = 0;

		// move direction
		if (teleDirection == 1)
			master->y += 0.5;
		else
			master->y -= 0.5;

		if (teleportHold >= 1000) {
			teleporting = 0;
			teleportHold = 0;
			master->renderOrder = 100;

			CGame::GetInstance()->GetCurrentScene()->GetCamera()->SetCurrentRegion(targetRegBound);
			master->SetPosition(desX, desY);
		}
	}

	//kick
	if (kick != 0) {
		kickTimer += dt;
		if (kickTimer >= MARIO_ATTACK_TIME) {
			kick = 0;
			kickTimer = 0;
		}
	}

	//death
	if (master->state == MARIO_STATE_DIE) {
		master->SetAlive(0);
		master->visible = 0;
	}

	//powerUp
	if (master->untouchable == 1) {
		master->untouchableTimer += dt;
		if (master->untouchableTimer >= 1500 && master->untouchableStep==0) {

			//power up reward
			if (powerUpLeaf == 1) {
				master->SetObjectState(new RacoonMario(master));
				pd->SetMariotype(RacoonMario::ObjectType);
				powerUpLeaf = 0;
			}
			else if (powerUpMushroom == 1) {
				master->SetObjectState(new BigMario(master));
				pd->SetMariotype(BigMario::ObjectType);
				powerUpMushroom = 0;
			}

			master->visible = 1;
			master->untouchableStep = 1;
		}

		if (master->untouchableTimer >= MARIO_UNTOUCHABLE_TIME && master->untouchableStep==1) {
			master->ResetUntouchable();
			master->untouchableStep = 0;
		}
	}
}

bool SmallMario::CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy)
{
	return false;
}

void SmallMario::MovementUpdate(DWORD dt)
{
	if( (master->untouchableTimer > 1500 && master->untouchable == 1) || master->untouchable==0) {
		master->ResetFlip();
		Keyboard* keyboard = CGame::GetInstance()->GetKeyboard();

		master->dt = dt;
		master->dx = master->vx * dt;
		master->dy = master->vy * dt;

		//cant fly
		// MOVE ON vx while flying tooo
		//if (master->state == MARIO_STATE_FLY) {
		//	master->vx = master->nx * MARIO_FLY_SPEED_X;
		//}

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

void SmallMario::JumpUpdate(DWORD dt)
{
	Keyboard* keyboard = CGame::GetInstance()->GetKeyboard();

	float height = 0;

	if (master->untouchable != 1) {
		// stop float , cannot fly anymore when drops to the ground
		if (master->isOnGround == 1) {
			master->ResetFloatTimer();
		}

		// if falls to ground -> set state idle
		if (master->state == MARIO_STATE_JUMP_FALL) {
			if (master->isOnGround == true) {
				master->SetState(MARIO_STATE_IDLE);
				master->vy = MARIO_GRAVITY * dt;
			}
		}

		//calc height
		if (master->state == MARIO_STATE_JUMP || master->state == MARIO_STATE_JUMP_HIGH || master->state == MARIO_STATE_JUMP_FALL || master->state == MARIO_STATE_FLY || master->state == MARIO_STATE_FLOAT) {
			height = abs(master->jumpStartPosition - master->y - master->vy * dt);
		}

		// fly for a time -> float
		if (master->state == MARIO_STATE_FLY) {
			if (height >= MARIO_FLY_FALL_POINT) {
				master->SetState(MARIO_STATE_FLOAT);
				master->vy = MARIO_FLY_PUSH;
			}
			else master->vy = -MARIO_FLY_PUSH - MARIO_GRAVITY * dt;
		}

		// float down
		if (master->state == MARIO_STATE_FLOAT) {
			//change float speed lightly
			// if float over 500ms -> cannot press S to fly anymore
			master->floatTimer += dt;
			master->vy = MARIO_FLY_PUSH;

			if (master->floatTimer < MARIO_FLOAT_TIME) {
				if (keyboard->IsKeyDown(DIK_S)) {
					master->ResetFloatTimer();

					if (height >= MARIO_FLY_FALL_POINT) {
						master->floatTimer = MARIO_FLOAT_TIME;
					}
					else {
						master->SetState(MARIO_STATE_FLY);
					}
				}
			}
	}
	
#pragma region JUMP & HIGH_JUMP & FALL
		if (master->state != MARIO_STATE_JUMP_FALL && master->state != MARIO_STATE_FLY && master->state != MARIO_STATE_FLOAT) {
			// continue jump when  ( not reaching fall point yet) HIGH & LOW
			if ((height < MARIO_JUMP_FALL_POINT && height != 0)
				|| (master->state == MARIO_STATE_JUMP_HIGH && height < MARIO_HIGH_JUMP_FALL_POINT)) {
				master->vy = -MARIO_JUMP_PUSH - MARIO_GRAVITY * dt;
			}

			//set state HIGH
			if (height > MARIO_BEGIN_HIGH_JUMP_HEIGHT && master->state == MARIO_STATE_JUMP) {
				if (master->state != MARIO_STATE_JUMP_HIGH)
					master->SetState(MARIO_STATE_JUMP_HIGH);
			}

			// if still holding S + pass fall point + not yet HIGH
			if (height > MARIO_JUMP_FALL_POINT && height < MARIO_BEGIN_HIGH_JUMP_HEIGHT) {
				if (keyboard->IsKeyDown(DIK_S)) {
					master->vy = -MARIO_JUMP_PUSH - MARIO_GRAVITY * dt;
				}
				else { //fall LOW
					master->vy = -MARIO_JUMP_PUSH / 2;
					master->SetState(MARIO_STATE_JUMP_FALL);
				}
			}

			//fall HIGH
			if (height > MARIO_HIGH_JUMP_FALL_POINT && master->state == MARIO_STATE_JUMP_HIGH) {
				master->vy = -MARIO_JUMP_PUSH / 2;
				master->SetState(MARIO_STATE_JUMP_FALL);
			}
		}
#pragma endregion
	}

}

void SmallMario::AttackUpdate(DWORD dt)
{
}

void SmallMario::RunPowerMeter(DWORD dt)
{
}

void SmallMario::PostCollisionUpdate(DWORD dt, vector<LPCOLLISIONEVENT>& coEventsResult, vector<LPCOLLISIONEVENT>& coEvents)
{
	if (coEvents.size() == 0) {
		if ((master->untouchableTimer>1500 && master->untouchable == 1) || master->untouchable==0) {
			master->UpdatePosition();
		}
	}

	if (coEvents.size() != 0) {
		float min_tx, min_ty, nx = 0, ny = 0;

		float rdx = 0;
		float rdy = 0;

		master->FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		if ((master->untouchableTimer > 1500 && master->untouchable == 1) || master->untouchable == 0) {
			master->x += min_tx * master->dx;
			master->y += min_ty * master->dy;
		 }

		if (nx != 0) master->vx = 0;
		if (ny != 0) { 
			master->vy = 0;
			if (ny < 0) {
				master->SetIsOnGround(true);
			}
			else if (ny > 0) {
				master->vy += master->gravity;
				master->SetState(MARIO_STATE_JUMP_FALL);
			}
			
		}
	}
}

void SmallMario::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT> coEvents)
{
	coEvents.clear();
	master->CollisionUpdate(dt, coObjects);
}

void SmallMario::BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult, vector<LPCOLLISIONEVENT> coEvents)
{
	PlayerData* pd = PlayerData::GetInstance();

	if (teleporting == 1)
		return;

	PostCollisionUpdate(dt, coEventsResult, coEvents); 

	if (master->untouchable != 1) {
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			switch (e->obj->GetObjectType()) {

			case PSwitch::ObjectType: 
			{
				PSwitch* p = dynamic_cast<PSwitch*>(e->obj);
				if (e->ny < 0) {
					p->activated = 1;
					int pp = CGame::GetInstance()->GetCurrentScene()->pSwitchActivated;
					if (pp != 1)
						CGame::GetInstance()->GetCurrentScene()->pSwitchActivated = 1;
				}
			}
			break;

			case CGoomba::ObjectType:
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				if (e->ny < 0) {
					master->vy = -MARIO_JUMP_DEFLECT_SPEED;
					pd->SetScore(pd->GetScore() + 100);
					EffectVault::GetInstance()->AddEffect(new ScoreFx("100", master->x, master->y));
				}

				if (e->nx != 0) {
					if (master->state != MARIO_STATE_DIE)
						master->SetState(MARIO_STATE_DIE);
					EffectVault::GetInstance()->AddEffect(new MarioDieFx(master->x, master->y));
				}
			}
			break;

			case VenusFireBall::ObjectType:
			{
				VenusFireBall* vn = dynamic_cast<VenusFireBall*>(e->obj);
				if (e->nx != 0 || e->ny!=0) {
					if (master->state != MARIO_STATE_DIE)
						master->SetState(MARIO_STATE_DIE);
					EffectVault::GetInstance()->AddEffect(new MarioDieFx(master->x, master->y));
				}
			}
			break;

			case Venus::ObjectType:
			{
				Venus* vn = dynamic_cast<Venus*>(e->obj);

				if (e->nx != 0 || e->ny!=0) {
					if (master->state != MARIO_STATE_DIE)
						master->SetState(MARIO_STATE_DIE);
					EffectVault::GetInstance()->AddEffect(new MarioDieFx(master->x, master->y));
				}
			}
			break;

			case Piranha::ObjectType:
			{
				Piranha* vn = dynamic_cast<Piranha*>(e->obj);

				if (e->nx != 0 || e->ny != 0) {
					if (master->state != MARIO_STATE_DIE)
						master->SetState(MARIO_STATE_DIE);
					EffectVault::GetInstance()->AddEffect(new MarioDieFx(master->x, master->y));
				}
			}
			break;

			case RedGoomba::ObjectType:
			{
				RedGoomba* rg = dynamic_cast<RedGoomba*>(e->obj);

				if (e->ny < 0) {
					master->vy = -MARIO_JUMP_DEFLECT_SPEED;
					pd->SetScore(pd->GetScore() + 200);
					EffectVault::GetInstance()->AddEffect(new ScoreFx("200", master->x, master->y));
				}
				else if (e->nx != 0) {
					if (master->state != MARIO_STATE_DIE)
						master->SetState(MARIO_STATE_DIE);
					EffectVault::GetInstance()->AddEffect(new MarioDieFx(master->x, master->y));
				}
			}
			break;

			case BoomBro::ObjectType:
			{
				BoomBro* rg = dynamic_cast<BoomBro*>(e->obj);

				if (e->ny < 0) {
					if (rg->state != BOOM_STATE_DIE) {
						rg->SetState(BOOM_STATE_DIE);
					}
					master->vy = -MARIO_JUMP_DEFLECT_SPEED;
					pd->SetScore(pd->GetScore() + 100);
					EffectVault::GetInstance()->AddEffect(new ScoreFx("100", master->x, master->y));
				}
				else if (e->nx != 0) {
					if (master->state != MARIO_STATE_DIE)
						master->SetState(MARIO_STATE_DIE);
					EffectVault::GetInstance()->AddEffect(new MarioDieFx(master->x, master->y));
				}
			}
			break;

			case Boomerang::ObjectType:
			{
				Boomerang* rg = dynamic_cast<Boomerang*>(e->obj);

				if (e->ny !=0 || e->nx!=0) {
					if (master->state != MARIO_STATE_DIE)
						master->SetState(MARIO_STATE_DIE);
					EffectVault::GetInstance()->AddEffect(new MarioDieFx(master->x, master->y));
				}
			}
			break;

			case MusicNote::ObjectType:
			{
				MusicNote* rg = dynamic_cast<MusicNote*>(e->obj);

				if (e->ny < 0) {
					if (rg->GetSpecialPortal() == 1) {
						master->vy = -MARIO_ULTRA_JUMP_DEFLECT;
						CGame::GetInstance()->GetCurrentScene()->GetCamera()->ReleasePlayer();
					}

					else
						master->vy = -0.2;
				}
			}
			break;

			case NormalKoopas::ObjectType:
			{
				NormalKoopas* rg = dynamic_cast<NormalKoopas*>(e->obj);

				if (e->ny < 0) {
					master->vy = -MARIO_JUMP_DEFLECT_SPEED;
					pd->SetScore(pd->GetScore() + 100);
					EffectVault::GetInstance()->AddEffect(new ScoreFx("100", master->x, master->y));
				}
				else if (e->nx != 0 || e->ny > 0) {
					if (master->state != MARIO_STATE_DIE)
						master->SetState(MARIO_STATE_DIE);
					EffectVault::GetInstance()->AddEffect(new MarioDieFx(master->x, master->y));
				}
			}
			break;

			case RedNormalKoopas::ObjectType:
			{
				RedNormalKoopas* rg = dynamic_cast<RedNormalKoopas*>(e->obj);

				if (e->ny < 0) {
					master->vy = -MARIO_JUMP_DEFLECT_SPEED;
					pd->SetScore(pd->GetScore() + 100);
					EffectVault::GetInstance()->AddEffect(new ScoreFx("100", master->x, master->y));
				}
				else if (e->nx != 0 || e->ny > 0) {
					if (master->state != MARIO_STATE_DIE)
						master->SetState(MARIO_STATE_DIE);
					EffectVault::GetInstance()->AddEffect(new MarioDieFx(master->x, master->y));
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
					if (master->state != MARIO_STATE_DIE)
						master->SetState(MARIO_STATE_DIE);
					EffectVault::GetInstance()->AddEffect(new MarioDieFx(master->x, master->y));
				}
			}
			break;

			case RedSlidingShell::ObjectType:
			{
				RedSlidingShell* rg = dynamic_cast<RedSlidingShell*>(e->obj);

				if (e->ny < 0) {
					master->vy = -MARIO_JUMP_DEFLECT_SPEED;
				}
				else if (e->nx != 0 || e->ny > 0) {
					if (master->state != MARIO_STATE_DIE)
						master->SetState(MARIO_STATE_DIE);
					EffectVault::GetInstance()->AddEffect(new MarioDieFx(master->x, master->y));
				}
			}
			break;

			case WingedKoopas::ObjectType:
			{
				WingedKoopas* rg = dynamic_cast<WingedKoopas*>(e->obj);

				if (e->ny < 0) {
					master->vy = -MARIO_JUMP_DEFLECT_SPEED;
					pd->SetScore(pd->GetScore() + 200);
					EffectVault::GetInstance()->AddEffect(new ScoreFx("200", master->x, master->y));
				}
				else if (e->nx != 0 || e->ny > 0) {
					if (master->state != MARIO_STATE_DIE)
						master->SetState(MARIO_STATE_DIE);
					EffectVault::GetInstance()->AddEffect(new MarioDieFx(master->x, master->y));
				}
			}
			break;

			case ShelledKoopas::ObjectType:
			{
				ShelledKoopas* rg = dynamic_cast<ShelledKoopas*>(e->obj);

				if (e->ny < 0) {
					master->vy = -MARIO_JUMP_DEFLECT_SPEED;
				}
				if (e->nx != 0) {
					kick = 1;
					if (e->nx > 0)
						rg->master->nx = 1;
					else rg->master->nx = -1;
				}
			}
			break;

			case RedShelledKoopas::ObjectType:
			{
				RedShelledKoopas* rg = dynamic_cast<RedShelledKoopas*>(e->obj);

				if (e->ny < 0) {
					master->vy = -MARIO_JUMP_DEFLECT_SPEED;
				}
				if (e->nx != 0) {
					kick = 1;

					if (e->nx > 0)
						rg->master->nx = 1;
					else rg->master->nx = -1;
				}
			}
			break;

			case Leaf::ObjectType:
			{
				Leaf* leaf = dynamic_cast<Leaf*>(e->obj);

				if (e->nx != 0 || e->ny != 0) {
					master->StartUntouchable();
					master->visible = 0;
					powerUpLeaf = 1;
					leaf->SetAlive(0);

					EffectVault::GetInstance()->AddEffect(new MarioTransform(master->x, master->y));
					pd->SetScore(pd->GetScore() + 100);
					
				}
			}
			break;

			case RedMushroom::ObjectType:
			{
				RedMushroom* gm = dynamic_cast<RedMushroom*>(e->obj);

				if (e->nx != 0 || e->ny != 0) {
					pd->SetScore(pd->GetScore() + 100);
					EffectVault::GetInstance()->AddEffect(new ScoreFx("100", master->x, master->y));
					master->StartUntouchable();
					master->visible = 0;
					powerUpMushroom = 1;
					gm->SetAlive(0);

					EffectVault::GetInstance()->AddEffect(new ToBigMario(master->x, master->y -35));
				}
			}
			break;

			case GreenMushroom::ObjectType:
			{
				GreenMushroom* gm = dynamic_cast<GreenMushroom*>(e->obj);

				if (e->nx != 0 || e->ny != 0) {
					gm->SetAlive(0);
				}
			}
			break;

			case CPortal::ObjectType:
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
			break;

			case BeginPortal::ObjectType:
			{
				BeginPortal* p = dynamic_cast<BeginPortal*>(e->obj);
				if (e->ny != 0) {
					desX = p->desX;
					desY = p->desY;
					targetRegBound = p->targetReg;

					if (p->GetCloudPortal() == 1) {
						CGame::GetInstance()->GetCurrentScene()->GetCamera()->SetCurrentRegion(targetRegBound);
						CGame::GetInstance()->GetCurrentScene()->GetCamera()->SetFocusOnPlayer(master);
						master->SetPosition(desX, desY);
					}
					else {
						teleporting = 1;

						master->renderOrder = 90;
						//determine vy of teleportation
						if (e->ny < 0) { //go down
							teleDirection = 1;
						}
						else teleDirection = -1; //go up
					}
				}
			}
			break;

			case EndPortal::ObjectType:
			{
				EndPortal* pp = dynamic_cast<EndPortal*>(e->obj);
				if (e->ny != 0 || e->nx!=0) {
					if (pp->direction == 1 ) {
						DebugOut(L"endport\n");
						teleporting = 1;
					}
				}
			}
			break;

			case EndCard::ObjectType:
			{
				EndCard* p = dynamic_cast<EndCard*>(e->obj);
				EffectVault::GetInstance()->AddEffect(new FlyingCard(p->x, p->y));
				p->SetAlive(0);
				
			}
			break;

			case Void::ObjectType:
			{
				Void* p = dynamic_cast<Void*>(e->obj);
				if (master->state != MARIO_STATE_DIE)
					master->SetState(MARIO_STATE_DIE);
				EffectVault::GetInstance()->AddEffect(new MarioDieFx(master->x, master->y));
				master->SetAlive(0);
			}
			break;

			case CBrick::ObjectType:
			{
				CBrick* p = dynamic_cast<CBrick*>(e->obj);
				if (e->ny > 0) {
					p->SetAlive(0);
					EffectVault::GetInstance()->AddEffect(new BrickBreak(p->x + 22, p->y + 22, 0.1, 0.7));
				}
			}
			break;

			}
		}
	}
}

void SmallMario::Render()
{
	if (master->visible == 0)
		return;

	InitAnimations();
	CAnimation* ani = this->animations["Idle"];

	switch (master->state) {
	case MARIO_STATE_IDLE:
		if (master->vy != 0) {
			ani = this->animations["Fall"];
		}
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
			if (master->powerMeter == PM_MAX) {
				ani = this->animations["HighSpeed"];
			}
			else
				ani = this->animations["Walk"];
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
	}

	if (kick == 1) {
		ani = this->animations["Kick"];
	}

	if (teleporting == 1) {
		ani = this->animations["TeleVer"];
	}

	int alpha = 255;
	if (master->untouchable==1)
		alpha = 128;

	master->SetFlipOnNormal(master->nx);
	int flip = master->flip;

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	float l, t, b, r;
	master->GetBoundingBox(l, t, r, b);

	if (master->state == MARIO_STATE_DIE) {
		camera->ReleasePlayer();
	}

	ani->SetPlayScale(1.5f);
	ani->Render(master->x - camera->GetX() + (r - l) / 2, master->y - camera->GetY() + (b - t) / 2, flip, alpha);
}

void SmallMario::OnKeyUp(int keyCode)
{
	//when not DIK left right -> not maxSpeed for run to trigger power meter duh
	master->isAtMaxRunSpeed = 0;

	if (keyCode == DIK_S) {
		// from fly to float when release S
		if (master->state == MARIO_STATE_FLY) {
			master->SetState(MARIO_STATE_FLOAT);
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

void SmallMario::OnKeyDown(int keyCode)
{
	if ( (master->untouchableTimer>1500 && master->untouchable==1 && teleporting != 1) || (master->untouchable==0 && teleporting!=1) ){
		switch (keyCode)
		{
		case DIK_X: {
			if (master->state != MARIO_STATE_JUMP && master->state != MARIO_STATE_JUMP_FALL) {
				if (master->isOnGround == true) {
					master->SetState(MARIO_STATE_JUMP);
					master->SetIsOnGround(false);
					master->GetPosY(master->jumpStartPosition);
					master->vy = -MARIO_JUMP_PUSH - MARIO_GRAVITY * master->dt;
				}
			}
		}
				  break;

		case DIK_S: {
			if (master->isOnGround == true) {
				if (master->powerMeter >= PM_MAX) {
					master->SetState(MARIO_STATE_FLY);
					master->vy = -MARIO_FLY_PUSH * 2 - MARIO_GRAVITY * master->dt;
				}
				else if(master->state!=MARIO_STATE_JUMP_FALL){
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
			if (master->state == MARIO_STATE_WALK)
				master->SetState(MARIO_STATE_RUN);
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

int SmallMario::GetObjectType()
{
	return ObjectType;
}
