#pragma once
#include "GameObject.h"
class ArrowPicker :
    public CGameObject
{
public:
    ArrowPicker();
    virtual void Update(DWORD dt);
    virtual void Render();
    virtual void OnKeyUp(int keyCode);
    virtual void OnKeyDown(int keyCode);
};

