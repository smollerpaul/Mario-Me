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
    //RenderBoundingBox();
}

int BeginPortal::GetObjectType()
{
    return ObjectType;
}

void BeginPortal::SetCloudPortal(int cloud)
{
    isCloud = 1;
}

int BeginPortal::GetCloudPortal()
{
    return isCloud;
}
