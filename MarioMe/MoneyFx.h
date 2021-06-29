#pragma once
#include "Effects.h"
class MoneyFx :
    public Effects
{
public:
    float flyUpSpeed = 0.6;
    float fallDownSpeed = 0.4f;
    float yBegin = 0;
    float gravity = 0.002f;

    int step = 1;

    MoneyFx(float xPos, float yPos, float aliveTime = 300);
    virtual void Update(DWORD dt);
    virtual void Render();
};

