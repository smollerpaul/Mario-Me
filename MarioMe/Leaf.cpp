#include "Leaf.h"
#include "Mario.h"
#include "PlayScene.h"

Leaf::Leaf(CPlayScene* ss)
{
    width = MUSHROOM_WIDTH;
    height = MUSHROOM_HEIGHT;
    gravity = LEAF_GRAVITY;
    nx = 1;
    this->scene = ss;
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
            y = yBegin - height - 50;
            vy = 0;
            scriptStep = 1;
        } 
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
    return false;
}

void Leaf::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    coEvents.clear();

    vector<LPGAMEOBJECT> coEventObjects;
    CMario* player = scene->GetPlayer();
    coEventObjects.push_back(player);

    CalcPotentialCollisions(&coEventObjects, coEvents);
    
}

void Leaf::BehaviorUpdate(DWORD dt)
{
    if (coEvents.size() == 0)
        CGameObject::UpdatePosition();

    if (coEvents.size() != 0) {
        float min_tx, min_ty, nx = 0, ny;
        float rdx = 0;
        float rdy = 0;

        FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

        x += min_tx * dx;
        y += min_ty * dy;

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
                if(alive!=0)
                   SetAlive(0);
            }
        }
        break;
        }
    }
}

int Leaf::GetObjectType()
{
    return ObjectType;
}
