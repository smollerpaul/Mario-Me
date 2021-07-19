#pragma once
#include "GameObject.h"

#define BOOM_STATE_WALK        10
#define BOOM_STATE_SHOOT       20
#define BOOM_STATE_DIE         30

#define BOOM_WIDTH             49
#define BOOM_HEIGHT            66
class CPlayScene;
class BoomBro :
    public CGameObject
{
public:
    float walkTime = 0;
    float attackTime = 0;
    float dieTimer = 0;
   
    CPlayScene* scene = nullptr;
    BoomBro(CPlayScene* ss);
    virtual void InitAnimations() override;
    virtual void Update(DWORD dt) override;
    virtual void Render()override;
    virtual int GetObjectType() override;
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;

    virtual void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    virtual void BehaviorUpdate(DWORD dt) override;

    static const int ObjectType = 93;
};

