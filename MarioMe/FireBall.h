#pragma once
#include "GameObject.h"
#include "SolidBlock.h"
#include "MarioConstants.h"

class Camera;

class FireBall :
    public CGameObject
{
public:
    CGameObject* player=nullptr;
    FireBall();
    FireBall(CGameObject* fplayer);
    virtual void InitAnimations() override;
    virtual void Update(DWORD dt) override;
    void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void BehaviorUpdate(DWORD dt);
    virtual void Render() override;

    virtual int GetObjectType() override;
    static const int ObjectType = 2000;

};

