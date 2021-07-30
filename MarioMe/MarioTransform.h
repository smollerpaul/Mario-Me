#pragma once
#include "Effects.h"
class MarioTransform :
    public Effects
{
public:
    MarioTransform(float xPos, float yPos, float aliveTime = 1400);
    virtual void Update(DWORD dt);
    virtual void Render();
};

