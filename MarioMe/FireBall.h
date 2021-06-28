#pragma once
#include "GameObject.h"
#include "SolidBlock.h"

#define GRAVITY                     0.002f
#define FIREBALL_SPEED              0.48f
#define FIREBALL_BOUNCE_PUSH        0.4f

#define FIREBALL_SIZE               24
#define FIREBALL_START_Y            35

class Camera;

class FireBall :
    public CGameObject
{
public:
    CGameObject* player;
         
    FireBall(CGameObject* fplayer);
    virtual void InitAnimations() override;
    virtual void Update(DWORD dt) override;
    void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void BehaviorUpdate(DWORD dt);
    virtual void Render() override;

    virtual int GetObjectType() override;
    static const int ObjectType = 2000;

};

