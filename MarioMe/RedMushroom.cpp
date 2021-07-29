#include "RedMushroom.h"
#include "Mario.h"

void RedMushroom::InitAnimations()
{
    if (this->animations.size() < 1) {
        this->animations["Default"] = CAnimations::GetInstance()->Get("ani-super-mushroom");
    }
}

void RedMushroom::BehaviorUpdate(DWORD dt)
{
    if (coEvents.size() == 0)
        CGameObject::UpdatePosition();

    if (coEvents.size() != 0) {
        float min_tx, min_ty, nx = 0, ny;
        float rdx = 0;
        float rdy = 0;

        FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

        x += min_tx * dx + nx * 0.2f;
        y += min_ty * dy + ny * 0.2f;

        if (nx != 0) vx = -vx;
        if (ny != 0) vy = 0;
    }

    for (UINT i = 0; i < coEventsResult.size(); i++)
    {
        LPCOLLISIONEVENT e = coEventsResult[i];

        switch (e->obj->GetObjectType()) {

        case CMario::ObjectType:
        {
            CMario* mario = dynamic_cast<CMario*>(e->obj);
            if (e->ny != 0 || e->nx != 0) {
                SetAlive(0);
            }
        }
        break;
        }
    }
}

int RedMushroom::GetObjectType()
{
    return ObjectType;
}
