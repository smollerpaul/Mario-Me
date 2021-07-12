#pragma once
#include "Effects.h"
class QBlockBounce :
    public Effects
{
public:
    float flyUpSpeed = 0.5f;
    float dyBounce = 0;
    float beginY = 0;
    int bounced = 0;

    QBlockBounce(float x, float y, float aliveTime = 1000);
    virtual void Update(DWORD dt) override;
    virtual void Render();
};

