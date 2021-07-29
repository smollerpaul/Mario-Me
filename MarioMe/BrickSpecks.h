#pragma once
#include "Effects.h"
class BrickSpecks :
    public Effects
{
public:
    float gravity = 0;
    float beginY = 0;
    float jumpHeight = 0;

    BrickSpecks(float xPos, float yPos, float vx, float vy, float aliveTime = 2000);
    virtual void Update(DWORD dt);
    virtual void Render();
};

