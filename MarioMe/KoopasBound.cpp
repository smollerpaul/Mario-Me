#include "KoopasBound.h"
#include "RedNormalKoopas.h"
#include "WingedKoopas.h"
#include "Goomba.h"


KoopasBound::KoopasBound()
{
}

void KoopasBound::Render()
{
   // RenderBoundingBox();
}

bool KoopasBound::CanGetThrough(CGameObject* gameObjToCollide, float coEventNx, float coEventNy)
{
    if (gameObjToCollide->GetObjectType() == RedNormalKoopas::ObjectType || 
        gameObjToCollide->GetObjectType() == NormalKoopas::ObjectType ||
        gameObjToCollide->GetObjectType() == WingedKoopas::ObjectType||
        gameObjToCollide->GetObjectType() == CGoomba::ObjectType)
        return false;
    return true;
}

int KoopasBound::GetObjectType()
{
    return ObjectType;
}
