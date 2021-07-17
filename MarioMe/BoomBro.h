#pragma once
#include "GameObject.h"
class BoomBro :
    public CGameObject
{
public:
    virtual int GetObjectType() override;

    static const int ObjectType = 81;
};

