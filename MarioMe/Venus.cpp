#include "Venus.h"
#include "Mario.h"
#include "Game.h"
#include "Camera.h"
#include "PlayScene.h"
#include "VenusFireBall.h"

Venus::Venus(CPlayScene *ss)
{
	width = VENUS_WIDTH;
	height = VENUS_HEIGHT;
	renderOrder = 90;
	this->scene = ss;
}

void Venus::SetPosition(float xPos, float yPos)
{
	this->x = xPos;
	this->y = yPos;

	beginY = yPos;
}

void Venus::CalcFireBallStat(float playerX, float playerY, float &fbVx, float &fbVy, float &fbX, float &fbY)
{
	float l, t, r, b;
	GetBoundingBox(l, t, r, b);

	float ballX = l + width / 2;
	float ballY = t + 15;

	float dxFromPlayer = playerX - ballX;
	float dyFromPlayer = playerY - ballY;

	float magDistance = sqrt(dxFromPlayer * dxFromPlayer + dyFromPlayer * dyFromPlayer);

	float angle = 0;

	if (abs(dxFromPlayer) > 48 * 6 || (0 < abs(dyFromPlayer) && abs(dyFromPlayer) < 96))
		angle = (dxFromPlayer > 0 ? 25 : 155) * (dyFromPlayer < 0 ? -1 : 1);
	else
		angle = (dxFromPlayer > 0 ? 45 : 135) * (dyFromPlayer < 0 ? -1 : 1);

	if (dyFromPlayer <0) {
		shootHeadDown = 0;
	}
	else shootHeadDown = 1;

	angle = angle * 3.14 / 180.0f;

	float magV = sqrt(cos(angle) * cos(angle) + sin(angle) * sin(angle));

	fbVx = VFIREBALL_SPEED * cos(angle) / magV;
	fbVy = VFIREBALL_SPEED * sin(angle) / magV;

	fbX = ballX;
	fbY = ballY;
}

void Venus::Update(DWORD dt)
{
	CMario* player = scene->GetPlayer();

	float px, py;
	player->GetPosition(px, py);

	if (scriptStep == 3) {
		waitTimer += dt;
		if (waitTimer >= VENUS_WAIT_TIME) {
			waitTimer = 0;
			scriptStep = 0;
		 }
	}
	
	if (scriptStep == 1) {
		revealTimer += dt;

		if (hasShot == 0 && state==VENUS_HEAD_UP) {
			if (abs(px - x) <= 48 * 6) {
				float ballVx, ballVy, ballX, ballY;

				CalcFireBallStat(px, py, ballVx, ballVy, ballX, ballY);

				VenusFireBall* fb = new VenusFireBall(ballVx, ballVy, ballX, ballY);
				CGame::GetInstance()->GetCurrentScene()->AddObject(fb);

				hasShot = 1;

			}
		}

		if (revealTimer > VENUS_REVEAL_TIME) {
			y += dt * VENUS_SPEED;
			hasShot = 0;
			SetState(VENUS_HEAD_DOWN);
			shootHeadDown = 1;

			if (y >=beginY) {
				y = beginY ;
				scriptStep = 3;
				revealTimer = 0;
			}
		}
		
	}
	
	if (scriptStep == 0) {
		SetState(VENUS_HEAD_UP);
		y -= dt* VENUS_SPEED;

		if (beginY - y >= height) {
			y = beginY - height;
			scriptStep = 1;
		}
	}

	if (scriptStep == 2) {
		y += dt * VENUS_SPEED;
		SetState(VENUS_HEAD_DOWN);

		if (y >= beginY) {
			y = beginY;
			scriptStep = 3;
			revealTimer = 0;
		}
	}

	if (player->x < this->x) {
		nx = -1;
	}
	else if (player->x >= this->x) {
		nx = 1;
	}

	DebugOut(L"shoot head down %d\n", shootHeadDown);
}

void Venus::Render()
{
	InitAnimations();

	CAnimation* ani = this->animations["RevealHeadUp"];

	if (isRedVenus == 1) {
		ani = this->animations["RedRevealHeadUp"];
	}
		
	if (state == VENUS_HEAD_DOWN) {
		if(isRedVenus==1)
			ani = this->animations["RedRevealHeadDown"];
		else
			ani = this->animations["RevealHeadDown"];
	}

	if (shootHeadDown == 0) {
		if(isRedVenus == 1)
			ani = this->animations["RedRevealHeadUp"];
		else 
			ani = this->animations["RevealHeadUp"];
	}
	else {
		if (isRedVenus == 1)
			ani = this->animations["RedRevealHeadDown"];
		else
			ani = this->animations["RevealHeadDown"];
	}

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	float l, t, b, r;
	GetBoundingBox(l, t, r, b);

	SetFlipOnNormalEnemy(nx);

	ani->Render(x - camera->GetX() + (r - l) / 2, y - camera->GetY() + (b - t) / 2, flip);

	//RenderBoundingBox();
}

bool Venus::CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy)
{
	if (obj->GetObjectType() == CMario::ObjectType)
		return true;
	else return false;
}

void Venus::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	
	//venus wont react to collision
}

void Venus::BehaviorUpdate(DWORD dt)
{
	float min_tx, min_ty, nx = 0, ny = 0;
	float rdx = 0;
	float rdy = 0;

	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
	

	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		switch (e->obj->GetObjectType()) {

		case SlidingShell::ObjectType:
		{
			SlidingShell* ss = dynamic_cast<SlidingShell*>(e->obj);
			if (e->nx != 0 || e->ny!=0)
			{
				if (scriptStep != 2) {
					scriptStep = 2;
				}
			}
		}
		break;

		case FireBall::ObjectType:
		{
			FireBall* fireball = dynamic_cast<FireBall*>(e->obj);
			if (e->ny != 0 || e->nx != 0)
			{
				if (scriptStep != 2) {
					scriptStep = 2;
				}
			}
		}
		break;

		case RacoonTail::ObjectType:
		{
			RacoonTail* tail = dynamic_cast<RacoonTail*>(e->obj);
			if (e->ny != 0 || e->nx != 0)
			{
				if (scriptStep!=2) {
					scriptStep = 2;
				}
				EffectVault::GetInstance()->AddEffect(new StarWhipTail(x, y + 20));
			}
		}
		break;
		}
	}

}

void Venus::InitAnimations()
{
	if (this->animations.size() < 1) {
		this->animations["RevealHeadUp"] = CAnimations::GetInstance()->Get("ani-green-venus-fire-trap-headup")->Clone();
		this->animations["RevealHeadDown"] = CAnimations::GetInstance()->Get("ani-green-venus-fire-trap-headdown")->Clone();
		this->animations["IdleHeadUp"] = CAnimations::GetInstance()->Get("ani-green-venus-fire-trap-headup-idle")->Clone();
		this->animations["IdleHeadDown"] = CAnimations::GetInstance()->Get("ani-green-venus-fire-trap-headdown-idle")->Clone();

		this->animations["RedRevealHeadUp"] = CAnimations::GetInstance()->Get("ani-red-venus-fire-trap-headup")->Clone();
		this->animations["RedRevealHeadDown"] = CAnimations::GetInstance()->Get("ani-red-venus-fire-trap-headdown")->Clone();
		this->animations["RedIdleHeadUp"] = CAnimations::GetInstance()->Get("ani-red-venus-fire-trap-headup-idle")->Clone();
		this->animations["RedIdleHeadDown"] = CAnimations::GetInstance()->Get("ani-red-venus-fire-trap-headdown-idle")->Clone();
	}
}

int Venus::GetObjectType()
{
    return ObjectType;
}
