#include "Leaf.h"
#include "Mario.h"

Leaf::Leaf()
{
    width = MUSHROOM_WIDTH;
    height = MUSHROOM_HEIGHT;
    gravity = LEAF_GRAVITY;
    nx = 1;

}

void Leaf::SetPosition(float x, float y)
{
    CGameObject::SetPosition(x, y);
    yBegin = y;
    xBegin = x;
}

void Leaf::InitAnimations()
{
    if (this->animations.size() < 1) {
        this->animations["Default"] = CAnimations::GetInstance()->Get("ani-super-leaf-red");
    }
}

void Leaf::Render()
{
    InitAnimations();
    CAnimation* ani = this->animations["Default"];

    if (visible == 0)
        return;

    Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
    float l, t, b, r;
    GetBoundingBox(l, t, r, b);

    SetFlipOnNormal(nx);
    int flip = this->flip;

    ani->Render(x - camera->GetX() + (r - l) / 2, y - camera->GetY() + (b - t) / 2, flip);
}

void Leaf::Update(DWORD dt)
{
  
    switch (scriptStep)
    {
        case 0: {
            vy -= LEAF_GROWUP_SPEED * dt;

        if (y + height + 50 <= yBegin) {
            y = yBegin - width - 50;
            vy = 0;
            scriptStep = 1;
        }
       /* else {
            if (visible == 0 && y + 10.0f <= yBegin) {
                visible = 1;
            }
            
        } */   
    }
        break;
        case 1:
            gravity= LEAF_GRAVITY;

            if (abs(x - xBegin) >= LEAF_REVEAL_DISTANCE) {
                nx = x < xBegin ? 1 : -1;
            }

            vx = LEAF_REVEAL_FORCE * dt * nx;

            break;
        default:
            break;
    }

   vy = min(vy + gravity * dt, LEAF_FALLING_VEL);

    CGameObject::Update(dt);
    //DebugOut(L"leaf : x: %f, y: %f, vy: %f . step: %d\n", x,y, vy, scriptStep);
}

bool Leaf::CanGetThrough(CGameObject* gameObjToCollide, float coEventNx, float coEventNy)
{
    return true;
}

void Leaf::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    CalcPotentialCollisions(coObjects, coEvents);
    if (coEvents.size() == 0)
        CGameObject::UpdatePosition();
}

void Leaf::BehaviorUpdate(DWORD dt)

{
    float min_tx, min_ty, nx = 0, ny;
    float rdx = 0;
    float rdy = 0;

    FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

    x += min_tx * dx + nx * 0.2f;
    y += min_ty * dy + ny * 0.2f;

    if (nx != 0) vx = -vx;
    if (ny != 0) vy = 0;

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
            DebugOut(L"Leaf dung mario\n");
        }
        break;
        }
    }
}

int Leaf::GetObjectType()
{
    return ObjectType;
}
