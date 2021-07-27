#pragma once
#include "Effects.h"
#include "Utils.h"
class BrickBreak :
    public Effects
{
public:

    float gravity = 0.002f;

    int step = 1;
    
    BrickBreak( float xPos, float yPos, float aliveTime = 300);
    virtual void Update(DWORD dt);
    virtual void Render();
};

