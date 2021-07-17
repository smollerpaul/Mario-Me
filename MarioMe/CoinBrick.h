#pragma once
#include "GameObject.h"
class CoinBrick :
    public CGameObject
{
public:

    float beginY = 0;
    float beginX = 0;
    float yLength = 0;
    float xLength = 0;
    int hitCount = 10;

    CoinBrick();

    virtual void InitAnimations() override;
    virtual void SetPosition(float x, float y) override;
    virtual void Render() override;
    virtual void Update(DWORD dt) override;

    void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void BehaviorUpdate(DWORD dt);

    virtual int GetObjectType() ;
    static const int ObjectType = 97;
};

