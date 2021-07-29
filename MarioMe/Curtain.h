#pragma once
#include "Effects.h"
class Curtain :
    public Effects
{
public:
    Curtain(float xPos, float yPos, float aliveTime = 1000);
    virtual void Update(DWORD dt);
    virtual void Render();
};

