#include "QuestionBlock.h"
#include "Camera.h"
#include "Game.h"
#include "Mario.h"
#include "SlidingShell.h"
#include "Leaf.h"
#include "MoneyFx.h"
#include "EffectVault.h"
#include "PSwitch.h"
#include "QBlockBounce.h"
#include "PlayScene.h"

QuestionBlock::QuestionBlock(CPlayScene* ss)
{
	vx = vy = 0;
	dx = dy = 0;
	gravity = 0;
	width = height = QB_SIZE;
	SetState(QB_STATE_ACTIVE);
	this->scene = ss;
}

void QuestionBlock::InitAnimations()
{
	if (this->animations.size() < 1) {
		this->animations["Active"] = CAnimations::GetInstance()->Get("ani-question-block")->Clone();
		this->animations["Frozen"] = CAnimations::GetInstance()->Get("ani-empty-block")->Clone();
		this->animations["Brick"] = CAnimations::GetInstance()->Get("ani-brick")->Clone();
	}
}

void QuestionBlock::SetPosition(float x, float y) {
	this->x = x;
	this->y = y;
	yStill = y;
}

void QuestionBlock::Render()
{
	InitAnimations();
	
	LPANIMATION ani = this->animations["Active"];

	if(state == QB_STATE_FROZEN || state == QB_STATE_BOUNCE)
		ani = this->animations["Frozen"];

	if (asBrick==1) {
		ani = this->animations["Brick"];
	}

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	float l, t, b, r;
	GetBoundingBox(l, t, r, b);

	if (visible == 0) {
		return;
	} 
	ani->Render(x - camera->GetX() + (r - l) / 2, y - camera->GetY() + (b - t) / 2, flip);

	RenderBoundingBox();
}

void QuestionBlock::Update(DWORD dt)
{
	if (state == QB_STATE_BOUNCE) {
		dyBounce += abs(dy);
		
		if (dyBounce >= QB_BOUNCE_HEIGHT) {
			vy += QB_GRAVITY * dt;
		}

		if (y > yStill) {
			vy = 0;
			y = yStill;
			SetState(QB_STATE_FROZEN);

			/*timeBounce += dt;
			if (timeBounce >= 800)
				visible = 1;*/
		}

	}
	CGameObject::Update(dt);
	
}

bool QuestionBlock::CanGetThrough(CGameObject* gameObjToCollide, float coEventNx, float coEventNy)
{
	return false;
}

void QuestionBlock::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
}

void QuestionBlock::BehaviorUpdate(DWORD dt)
{
	PlayerData* pd = PlayerData::GetInstance();
	if (coEvents.size() == 0) {
		CGameObject::UpdatePosition();
	}
	if (coEvents.size() != 0) {
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		DebugOut(L"QBlock touches: %d\n", coEventsResult[0]->obj->GetObjectType());
	}
	
	for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			//error touch leaf -> leaf disappeared but collided with block so now id is empty
				switch (e->obj->GetObjectType()) {

				case CMario::ObjectType:
				{
					CMario* mario = dynamic_cast<CMario*>(e->obj);
					if (e->ny < 0)
					{
						if (state == QB_STATE_ACTIVE) {
							vy = -QB_BOUNCE_SPEED;
							SetState(QB_STATE_BOUNCE);

							//visible = 0;
							//EffectVault::GetInstance()->AddEffect(new QBlockBounce(this->x, this->y,800));

							if (reward == COIN_PRIZE) {
								EffectVault::GetInstance()->AddEffect(new MoneyFx(this->x, this->y));
							}

							if (reward == LEAF_PRIZE) {
								Leaf* leaf = new Leaf(scene);
								leaf->SetPosition(x, y - 48);
								CGame::GetInstance()->GetCurrentScene()->AddObject(leaf);
							}

							if (reward == GMUSH_PRIZE) {
								GreenMushroom* gm = new GreenMushroom();
								gm->SetPosition(x, y - 45);
								CGame::GetInstance()->GetCurrentScene()->AddObject(gm);
							}
							pd->SetScore(pd->GetScore() + 100);
							pd->SetCoins(pd->GetCoins() + 1);
						}
					}
				}
				break;

				case SlidingShell::ObjectType:
				{
					SlidingShell* ss = dynamic_cast<SlidingShell*>(e->obj);
					if (e->nx != 0)
					{
						if (state == QB_STATE_ACTIVE) {
							if (state != QB_STATE_FROZEN)
								SetState(QB_STATE_FROZEN);
							pd->SetScore(pd->GetScore() + 100);
						}
					}
				}
				break;

				case RacoonTail::ObjectType:
				{
					RacoonTail* tail = dynamic_cast<RacoonTail*>(e->obj);
					if (e->nx != 0)
					{
						if (state == QB_STATE_ACTIVE) {

							if (state != QB_STATE_FROZEN)
								SetState(QB_STATE_FROZEN);

							if (reward == PSWITCH_PRIZE) {
								PSwitch* ps = new PSwitch();
								ps->SetPosition(this->x, this->y);
								CGame::GetInstance()->GetCurrentScene()->AddObject(ps);
							}

							if (reward == LEAF_PRIZE) {
								Leaf* leaf = new Leaf(scene);
								leaf->SetPosition(x, y);
								CGame::GetInstance()->GetCurrentScene()->AddObject(leaf);
							}

							pd->SetScore(pd->GetScore() + 100);
						}
					}
				}
				break;

				default: break;

				}
			}
}

void QuestionBlock::SetReward(int blockReward)
{
	this->reward = blockReward;
}

void QuestionBlock::SetAsBrick(int asBrick)
{
	this->asBrick = asBrick;
}

int QuestionBlock::GetObjectType()
{
    return ObjectType;
}
