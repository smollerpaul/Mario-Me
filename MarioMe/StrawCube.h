#pragma once
#include "GameObject.h"

#define STRAW_STATE_ACTIVE  100
#define STRAW_STATE_EMPTY   200


class Camera;
class CPlayScene;
class StrawCube :
    public CGameObject
{
public:
    int cubeReward = 0;
    float beginX = 0;
    float xLength = 0;
    CPlayScene* scene = nullptr;

    StrawCube(CPlayScene* ss);

    virtual void InitAnimations() override;
    virtual void SetPosition(float x, float y) override;
    virtual void Render() override;
    virtual void Update(DWORD dt) override;

    void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void BehaviorUpdate(DWORD dt);
    void SetReward(int blockReward);
    
    virtual int GetObjectType() override;
    static const int ObjectType = 95;

};

