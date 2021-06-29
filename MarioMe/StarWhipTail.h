#pragma once
#include "PoofFx.h"
class StarWhipTail :
    public Effects
{
public:
    StarWhipTail(float xPos, float yPos, float aliveTime = 500);
    virtual void Update(DWORD dt);
    virtual void Render() override;
};

