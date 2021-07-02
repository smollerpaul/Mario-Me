#pragma once
#include "GameObject.h"

class Void :
    public CGameObject
{
public:
    Void();
    void Update(DWORD dt) override;
    void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void BehaviorUpdate(DWORD dt) override;
    void Render() override;
    virtual int GetObjectType() override;
    static const int ObjectType = 90;
};

