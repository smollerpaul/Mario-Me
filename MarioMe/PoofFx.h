#pragma once
#include "Effects.h"
class PoofFx :
    public Effects
{
public:
    PoofFx();
    PoofFx(float xPos, float yPos, float aliveTime = 500);
    virtual void Update(DWORD dt);
    virtual void Render();
};

