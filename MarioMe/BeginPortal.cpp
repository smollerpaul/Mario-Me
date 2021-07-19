#include "BeginPortal.h"

BeginPortal::BeginPortal(float destinationX, float destinationY, int targetRegion, int direction)
{
    this->desX = destinationX;
    this->desY = destinationY;
    this->targetReg = targetRegion;
    this->direction = direction;
    this->renderOrder = 99;
}

void BeginPortal::Render()
{
    RenderBoundingBox();
}

int BeginPortal::GetObjectType()
{
    return ObjectType;
}
