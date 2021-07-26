#pragma once
#include "GameObject.h"
class MusicNote :
    public CGameObject
{
public:
   
    float beginY = 0;
    float beginX = 0;
    float yLength = 0;
    float xLength = 0;

    int specialPortal = 0;
    
    MusicNote();

    virtual void InitAnimations() override;
    virtual void SetPosition(float x, float y) override;
    virtual void Render() override;
    virtual void Update(DWORD dt) override;

    void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void BehaviorUpdate(DWORD dt);
    void SetSpecialPortal(int special);
    int GetSpecialPortal();
    virtual int GetObjectType() override;
    static const int ObjectType = 96;
};

