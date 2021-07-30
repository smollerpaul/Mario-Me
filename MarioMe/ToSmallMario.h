#pragma once
#include "Effects.h"
class ToSmallMario :
    public Effects
{
public:
    ToSmallMario(float xPos, float yPos, float aliveTime = 1500);

    virtual void Update(DWORD dt) override;
    virtual void Render() override;
};

