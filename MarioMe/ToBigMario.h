#pragma once
#include "Effects.h"
class ToBigMario :
    public Effects
{
public:
    ToBigMario(float xPos, float yPos, float aliveTime = 1500);
    virtual void Update(DWORD dt);
    virtual void Render();
};

