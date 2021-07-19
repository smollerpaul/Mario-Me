#pragma once
#include "GameObject.h"
class EndPortal :
    public CGameObject
{
public:
    int direction = 0;
    EndPortal(int dir);
    virtual void Render() override;
    virtual int GetObjectType() override;
    static const int ObjectType = 766;
};

