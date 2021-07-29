#pragma once
#include "Effects.h"
class FlyingCard :
    public Effects
{
public:
    float flyUpSpeed = 0.5f;

    FlyingCard( float x, float y, float aliveTime = 500);
    virtual void Update(DWORD dt) override;
    virtual void Render();

};

