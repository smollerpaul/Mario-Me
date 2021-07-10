#pragma once
#include "GameObject.h"
#include "Venus.h"
#include "Scene.h"

class VenusFireBall :
    public CGameObject
{
public:
    float beginX = 0;
    float beginY = 0;

    VenusFireBall(float ballVx, float ballVy, float x, float y);
    virtual void Update(DWORD dt) override;
    void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void BehaviorUpdate(DWORD dt);
    virtual void Render() override;

    virtual int GetObjectType() override;
    static const int ObjectType = 2024;
};

