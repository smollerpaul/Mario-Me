#pragma once
#include "GameObject.h"
#define TAIL_SPEED 0.55

#define TAIL_ALIVE_TIME 250
class Camera;

class RacoonTail :
    public CGameObject
{
public:
    float aliveTimer = 0;
    CGameObject* player = nullptr;

    RacoonTail();
    RacoonTail(CGameObject* fplayer);
    virtual void Update(DWORD dt) override;
    void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void BehaviorUpdate(DWORD dt);
    virtual void Render() override;

    virtual int GetObjectType() override;
    static const int ObjectType = 2002;
};

