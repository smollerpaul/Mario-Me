#include "EndPortal.h"

EndPortal::EndPortal(int dir)
{
    this->direction = dir;
    this->renderOrder = 99;
}

void EndPortal::Render()
{
    RenderBoundingBox();
}

int EndPortal::GetObjectType()
{
    return ObjectType;
}
