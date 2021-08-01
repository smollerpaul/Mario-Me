#pragma once
#include "NormalGoomba.h"

class WingedGoomba :
    public NormalGoomba
{
public:
    float walkTime = 0;
    float jumpHeight = 0;
    WingedGoomba(CGoomba* master);
    virtual void Update(DWORD dt);
    virtual void Render();
    virtual void BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult, vector<LPCOLLISIONEVENT> coEvents);
    virtual int GetObjectType();
};

