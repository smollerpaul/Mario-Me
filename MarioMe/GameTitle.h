#pragma once
#include "Effects.h"
class GameTitle :
    public Effects
{
public:
    GameTitle( float xPos, float yPos, float aliveTime = 100000);
    virtual void Update(DWORD dt);
    virtual void Render();
};

