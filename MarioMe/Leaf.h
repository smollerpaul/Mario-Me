#pragma once
#include "GameObject.h"

#define LEAF_GRAVITY 0.0012f;
const int LEAF_AMPLITUDE = 48; // px
const int LEAF_ROUNDTRIP_TIME = 800;
const float LEAF_REVEAL_DISTANCE = 72; // px
const float LEAF_REVEAL_FORCE = 0.005f;
const float LEAF_FALLING_VEL = 0.05f;
const float LEAF_GROWUP_SPEED = 0.012f;
class CPlayScene;
class Leaf :
    public CGameObject
{
public:
	int visible = 1;
	int scriptStep = 0;
	float yBegin = 0;
	float xBegin = 0;
	float deadTimer = 0;

	CPlayScene* scene = nullptr;

	Leaf(CPlayScene* ss);
	virtual void SetPosition(float x, float y) override;
	virtual void InitAnimations() override;
	virtual void Render() override;
	virtual void Update(DWORD dt) override;
	virtual bool CanGetThrough(CGameObject* gameObjToCollide, float coEventNx, float coEventNy) override;

	void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void BehaviorUpdate(DWORD dt);

	virtual int GetObjectType();
	static const int ObjectType = 4002;

};

