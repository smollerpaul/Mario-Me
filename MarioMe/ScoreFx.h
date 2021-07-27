#pragma once
#include "Effects.h"
class ScoreFx :
    public Effects
{
public:
    float flyUpSpeed = 0.3;
    float fallDownSpeed = 0.4f;
    float yBegin = 0;
    float gravity = 0.002f;

    int step = 1;
    string score = "100";

    ScoreFx(string score, float xPos, float yPos, float aliveTime = 300);
    virtual void Update(DWORD dt);
    virtual void Render();
};

