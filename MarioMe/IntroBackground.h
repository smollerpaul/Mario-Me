#pragma once
#include "Effects.h"
class Text;
class IntroBackground :
    public Effects
{
public:
    Text* player1 = nullptr;
    Text* player2 = nullptr;

    int textCue = 0;
    float textCueTimer = 0;

    IntroBackground(float aliveTime = 100000);
    virtual void Update(DWORD dt);
    virtual void Render();
};

