#pragma once
#include "GameObject.h"
#define BOOMERANG_VX            0.3f
#define BOOMERANG_VY            0.05f

#define BOOMERANG_RETURN_TIME   1000

#define BOOMERANG_STATE_SHOOT   10
#define BOOMERANG_STATE_RETURN   11

class BoomBro;
class Boomerang :
    public CGameObject
{
public:
    float beginX = 0;
    float beginY = 0;

    float shootTimer = 0;
    float aliveTime = 0;
    BoomBro* master = nullptr;

    Boomerang(BoomBro* master);
    virtual void Update(DWORD dt) override;
    void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void BehaviorUpdate(DWORD dt);

    virtual void Render() override;

    virtual int GetObjectType() override;
    static const int ObjectType = 919;
};

