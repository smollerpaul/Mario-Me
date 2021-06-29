#pragma once
#include "Effects.h"
class MarioDieFx :
    public Effects
{
public:
    float gravity = 0.002f;
    float bounceSpeed = 0.5;
    float fallDownSpeed = 0.5;

    float yBegin = 0;

    int step = 1;

    MarioDieFx(float xPos, float yPos, float aliveTime = 10000);
    virtual void Update(DWORD dt);
    virtual void Render();
};

