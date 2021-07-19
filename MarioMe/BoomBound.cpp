#include "BoomBound.h"
#include "BoomBro.h"

BoomBound::BoomBound()
{
}

void BoomBound::Render()
{
  //  RenderBoundingBox();
}

bool BoomBound::CanGetThrough(CGameObject* gameObjToCollide, float coEventNx, float coEventNy)
{
    if (gameObjToCollide->GetObjectType() == BoomBro::ObjectType)
        return false;
    return true;
}

int BoomBound::GetObjectType()
{
    return ObjectType;
}
