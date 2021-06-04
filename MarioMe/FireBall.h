#pragma once
#include "GameObject.h"

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
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
    void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects,
        vector<LPCOLLISIONEVENT> coEvents,
        vector<LPCOLLISIONEVENT>& coEventsResult);
    void BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult);

    
    virtual void Render() override;
    void ResetAll();

    virtual int GetObjectType() override;
    static const int ObjectType = 2000;

};

