#pragma once
#include "GameObject.h"

#define VENUS_SPEED			0.13f;
#define VENUS_WIDTH			48
#define VENUS_HEIGHT		96
#define VENUS_REVEAL_TIME	5000
#define VENUS_WAIT_TIME		1000
#define VENUS_HEAD_UP		10
#define VENUS_HEAD_DOWN		11

class CPlayScene;
class Venus :
    public CGameObject
{
public:
	float revealTimer = 0;
	float waitTimer = 0;
	int scriptStep = 0;

	CPlayScene* scene = nullptr;
	float beginY = 0;
	int hasShot = 0;

	int isRedVenus = 0;
	int shootHeadDown = 1;

	Venus(CPlayScene* ss);
	virtual void SetPosition(float xPos, float yPos) override;
	void CalcFireBallStat(float playerX, float playerY, float& fbVx, float& fbVy, float& fbX, float& fbY);

	virtual void Update(DWORD dt) override;
	virtual void Render() override;
	virtual bool CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy) override;

	void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void BehaviorUpdate(DWORD dt);

	virtual void InitAnimations() override;

	virtual int GetObjectType() override;
	static const int ObjectType = 85;
};

