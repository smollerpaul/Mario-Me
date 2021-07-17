#pragma once
#include "GameObject.h"
class BoomBound :
    public CGameObject
{
public:
    BoomBound();
    virtual void Render() override;
    virtual bool CanGetThrough(CGameObject* gameObjToCollide, float coEventNx, float coEventNy) override;
    virtual int GetObjectType() override;

    static const int ObjectType = 80;
};

