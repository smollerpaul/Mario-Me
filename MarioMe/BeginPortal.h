#pragma once
#include "GameObject.h"
class BeginPortal :
    public CGameObject
{
public:
    float desX = 0;
    float desY = 0;
    int targetReg = 0;
    int direction = 0;

    BeginPortal( float destinationX, float destinationY, int targetRegion, int direction);
    virtual void Render() override;
    virtual int GetObjectType() override;

    static const int ObjectType = 765;
};

