#pragma once
#include "Effects.h"
class OverworldTrees :
    public Effects
{
public:
    OverworldTrees();
    OverworldTrees(float xPos, float yPos, float aliveTime = 500);
    virtual void Update(DWORD dt);
    virtual void Render();
};

