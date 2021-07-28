#pragma once
#include "Effects.h"
#include "Utils.h"

class BrickSpecks;
class BrickBreak :
    public Effects
{
public:

    BrickSpecks* b1=nullptr;
    BrickSpecks* b2 = nullptr;
    BrickSpecks* b3 = nullptr;
    BrickSpecks* b4 = nullptr;
    BrickSpecks* b5 = nullptr;
    
    BrickBreak(float xPos, float yPos, float vx=0.1f, float vy = 0.2f, float aliveTime = 300);
    virtual void Update(DWORD dt);
    virtual void Render();
};

