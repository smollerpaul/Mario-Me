#include "BigMario.h"

BigMario::BigMario()
{
}

BigMario::BigMario(CMario* masterObj)
{
	this->master = masterObj;

	if (master->height != 81) {
		master->SetPosition(master->x, master->y - 40);
	}

	master->SetSize(MARIO_WIDTH, MARIO_HEIGHT);
}

void BigMario::InitAnimations()
{
	if (this->animations.size() < 1) {
		this->animations["Idle"] = CAnimations::GetInstance()->Get("ani-big-mario-idle")->Clone();
		this->animations["Walk"] = CAnimations::GetInstance()->Get("ani-big-mario-walk")->Clone();
		this->animations["Run"] = CAnimations::GetInstance()->Get("ani-big-mario-run")->Clone();
		this->animations["HighSpeed"] = CAnimations::GetInstance()->Get("ani-big-mario-high-speed")->Clone();
		this->animations["Jump"] = CAnimations::GetInstance()->Get("ani-big-mario-jump")->Clone();
		this->animations["HighJump"] = CAnimations::GetInstance()->Get("ani-big-mario-high-jump")->Clone();
		this->animations["Fly"] = CAnimations::GetInstance()->Get("ani-big-mario-high-jump")->Clone();
		this->animations["Fall"] = CAnimations::GetInstance()->Get("ani-big-mario-fall")->Clone();
		this->animations["Float"] = CAnimations::GetInstance()->Get("ani-big-mario-high-jump")->Clone();
		this->animations["Skid"] = CAnimations::GetInstance()->Get("ani-big-mario-skid")->Clone();
		this->animations["Crouch"] = CAnimations::GetInstance()->Get("ani-big-mario-crouch")->Clone();
		this->animations["Kick"] = CAnimations::GetInstance()->Get("ani-big-mario-kick")->Clone();

		this->animations["Hold"] = CAnimations::GetInstance()->Get("ani-big-mario-hold")->Clone();
		this->animations["HoldIdle"] = CAnimations::GetInstance()->Get("ani-big-mario-hold-idle")->Clone();
		this->animations["HoldFall"] = CAnimations::GetInstance()->Get("ani-big-mario-hold-fall")->Clone();

		this->animations["TeleVer"] = CAnimations::GetInstance()->Get("ani-big-mario-idle-front")->Clone();
		this->animations["TeleHor"] = CAnimations::GetInstance()->Get("ani-big-mario-walk")->Clone();
		this->animations["Shrink"] = CAnimations::GetInstance()->Get("ani-mario-shrink-down")->Clone();
		this->animations["Spin"] = CAnimations::GetInstance()->Get("ani-raccoon-mario-spin")->Clone();
	}
}

void BigMario::Update(DWORD dt)
{
	PlayerData* pd = PlayerData::GetInstance();
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
	
	if (kick != 0) {
		kickTimer += dt;
		if (kickTimer >= MARIO_ATTACK_TIME) {
			kick = 0;
			kickTimer = 0;
		}
	}

	if (master->state == MARIO_STATE_DIE) {
		master->SetAlive(0);
		master->visible = 0;
	}

	if (master->untouchable == 1) {
		master->untouchableTimer += dt;

		if (master->untouchableTimer >= MARIO_UNTOUCHABLE_TIME) {
			master->ResetUntouchable();

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
			else {
				master->SetObjectState(new SmallMario(master));
				pd->SetMariotype(SmallMario::ObjectType);
			}

			master->visible = 1;
		}
	}

}

bool BigMario::CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy)
{
	return master->untouchable = 1;
}

void BigMario::MovementUpdate(DWORD dt)
{
	string currentScene= CGame::GetInstance()->GetCurrentScene()->GetSceneId();

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
		if (currentScene.compare("intro") != 0) {
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

void BigMario::JumpUpdate(DWORD dt)
{
	SmallMario::JumpUpdate(dt);
}

void BigMario::AttackUpdate(DWORD dt)
{
}

void BigMario::BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult, vector<LPCOLLISIONEVENT> coEvents)
{
	PlayerData* pd = PlayerData::GetInstance();
	Keyboard* keyboard = CGame::GetInstance()->GetKeyboard();

	if (teleporting == 1)
		return;

	SmallMario::PostCollisionUpdate(dt, coEventsResult, coEvents);
	if (master->untouchable != 1) {
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			switch (e->obj->GetObjectType()) {

			case CMario::ObjectType:
			{
				CMario* mm = dynamic_cast<CMario*>(e->obj);

				if (e->ny < 0) {
					master->vy = -MARIO_JUMP_DEFLECT_SPEED;
				}

				if (e->ny > 0) {
					master->SetState(MARIO_STATE_CROUCH);
					master->vx = 0;
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
					if (keyboard->IsKeyDown(DIK_A)) {
						master->SetHoldingGreenShell(rg);
					}
					else {
						kick = 1;
					}
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
					if (keyboard->IsKeyDown(DIK_A)) {
						master->SetHoldingRedShell(rg);
					}
					else {
						kick = 1;
					}
				}
			}
			break;

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

			case CBrick::ObjectType:
			{
				CBrick* p = dynamic_cast<CBrick*>(e->obj);
				if (e->ny > 0)
					p->SetAlive(0);
				//EffectVault::GetInstance()->AddEffect(new MarioDieFx(master->x, master->y));
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
					if (master->untouchable == 0) {
						master->StartUntouchable();
						master->visible = 0;
					}
					EffectVault::GetInstance()->AddEffect(new ToSmallMario(master->x, master->y, MARIO_UNTOUCHABLE_TIME));
				}
			}
			break;

			case BoomBro::ObjectType:
			{
				BoomBro* goomba = dynamic_cast<BoomBro*>(e->obj);

				if (e->ny < 0) {
					master->vy = -MARIO_JUMP_DEFLECT_SPEED;
					pd->SetScore(pd->GetScore() + 100);
					EffectVault::GetInstance()->AddEffect(new ScoreFx("100", master->x, master->y));
				}

				if (e->nx != 0) {
					if (master->untouchable == 0) {
						master->StartUntouchable();
						master->visible = 0;
					}
					EffectVault::GetInstance()->AddEffect(new ToSmallMario(master->x, master->y, MARIO_UNTOUCHABLE_TIME));
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
						master->vy = -MARIO_JUMP_DEFLECT_SPEED;
				}
			}
			break;

			case Boomerang::ObjectType:
			{
				BoomBro* goomba = dynamic_cast<BoomBro*>(e->obj);

				if (e->ny != 0 || e->nx!=0) {
				
					if (master->untouchable == 0) {
						master->StartUntouchable();
						master->visible = 0;
					}
					EffectVault::GetInstance()->AddEffect(new ToSmallMario(master->x, master->y, MARIO_UNTOUCHABLE_TIME));
				}
			}
			break;

			case Venus::ObjectType:
			{
				Venus* vn = dynamic_cast<Venus*>(e->obj);

				if (e->nx != 0 || e->ny != 0) {
					if (master->untouchable == 0) {
						master->StartUntouchable();
						master->visible = 0;
					}
					EffectVault::GetInstance()->AddEffect(new MarioTransform(master->x, master->y + 25, MARIO_UNTOUCHABLE_TIME));
				}
			}
			break;

			case VenusFireBall::ObjectType:
			{
				VenusFireBall* vn = dynamic_cast<VenusFireBall*>(e->obj);

				if (e->nx != 0 || e->ny != 0) {
					if (master->untouchable == 0) {
						master->StartUntouchable();
						master->visible = 0;
					}
					EffectVault::GetInstance()->AddEffect(new MarioTransform(master->x, master->y + 25, MARIO_UNTOUCHABLE_TIME));
				}
			}
			break;

			case Piranha::ObjectType:
			{
				Piranha* vn = dynamic_cast<Piranha*>(e->obj);

				if (e->nx != 0 || e->ny != 0) {
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
					pd->SetScore(pd->GetScore() + 200);
					EffectVault::GetInstance()->AddEffect(new ScoreFx("200", master->x, master->y));
				}
				else if (e->nx != 0) {
					if (master->untouchable == 0) {
						master->StartUntouchable();
						master->visible = 0;
					}
					EffectVault::GetInstance()->AddEffect(new ToSmallMario(master->x, master->y, MARIO_UNTOUCHABLE_TIME));
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
					if (master->untouchable == 0) {
						master->StartUntouchable();
						master->visible = 0;
					}
					EffectVault::GetInstance()->AddEffect(new ToSmallMario(master->x, master->y, MARIO_UNTOUCHABLE_TIME));
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
					if (master->untouchable == 0) {
						master->StartUntouchable();
						master->visible = 0;
					}
					EffectVault::GetInstance()->AddEffect(new ToSmallMario(master->x, master->y, MARIO_UNTOUCHABLE_TIME));
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
					if(master->untouchable == 0) {
						master->StartUntouchable();
						master->visible = 0;
					}
					EffectVault::GetInstance()->AddEffect(new ToSmallMario(master->x, master->y, MARIO_UNTOUCHABLE_TIME));
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
					if (master->untouchable == 0) {
						master->StartUntouchable();
						master->visible = 0;
					}
					EffectVault::GetInstance()->AddEffect(new ToSmallMario(master->x, master->y, MARIO_UNTOUCHABLE_TIME));
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
					if (master->untouchable == 0) {
						master->StartUntouchable();
						master->visible = 0;
					}
					EffectVault::GetInstance()->AddEffect(new ToSmallMario(master->x, master->y, MARIO_UNTOUCHABLE_TIME));
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
				EffectVault::GetInstance()->AddEffect(new FlyingCard(p->x, p->y));
				p->SetAlive(0);
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

			case Void::ObjectType:
			{
				Void* p = dynamic_cast<Void*>(e->obj);
				if (master->state != MARIO_STATE_DIE)
					master->SetState(MARIO_STATE_DIE);
				EffectVault::GetInstance()->AddEffect(new MarioDieFx(master->x, master->y));
				master->SetAlive(0);
			}
			break;

			case GreenMushroom::ObjectType:
			{
				GreenMushroom* gm = dynamic_cast<GreenMushroom*>(e->obj);

				if (e->nx != 0 || e->ny != 0) {
					master->StartUntouchable();
					master->visible = 0;
					powerUpMushroom = 1;
					EffectVault::GetInstance()->AddEffect(new PoofFx(master->x, master->y - 35, MARIO_UNTOUCHABLE_TIME));
				}
			}
			break;

			}
			
		}
	}
}

void BigMario::OnKeyUp(int keyCode)
{
	//when not DIK left right -> not maxSpeed for run to trigger power meter duh
	string currentScene = CGame::GetInstance()->GetCurrentScene()->GetSceneId();
	if (currentScene.compare("intro") != 0) {
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
}

void BigMario::OnKeyDown(int keyCode)
{
	string currentScene = CGame::GetInstance()->GetCurrentScene()->GetSceneId();
	if (currentScene.compare("intro") != 0) {
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
}

void BigMario::Render()
{
	InitAnimations();
	CAnimation* ani = this->animations["Idle"];

	//dont render when transforming
	if (master->visible == 0)
		return;

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

	case MARIO_STATE_SHRINK:
		ani = this->animations["Shrink"];
		break;
	}

	if (kick == 1) {
		ani = this->animations["Kick"];
	}

	if (teleporting == 1) {
		ani = this->animations["TeleVer"];
	}

	if (master->holdingRedShell != nullptr || master->holdingGreenShell!=nullptr) {
		ani = this->animations["Hold"];

		if (master->vx != 0) {
			ani = this->animations["HoldFall"];
		}
	}

	int alpha = 255;
	if (master->untouchable)
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

