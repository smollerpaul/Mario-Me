#include "KoopasBound.h"
#include "RedNormalKoopas.h"

KoopasBound::KoopasBound()
{
}

void KoopasBound::Render()
{
   // RenderBoundingBox();
}

bool KoopasBound::CanGetThrough(CGameObject* gameObjToCollide, float coEventNx, float coEventNy)
{
    if (gameObjToCollide->GetObjectType() == RedNormalKoopas::ObjectType)
        return false;
    return true;
}

int KoopasBound::GetObjectType()
{
    return ObjectType;
}
