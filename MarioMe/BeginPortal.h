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
    int isCloud = 0;

    BeginPortal( float destinationX, float destinationY, int targetRegion, int direction);
    virtual void Render() override;
    virtual int GetObjectType() override;

    void SetCloudPortal(int cloud);
    int GetCloudPortal();

    static const int ObjectType = 765;
};

