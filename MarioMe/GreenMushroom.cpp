#include "GreenMushroom.h"
#include "Camera.h"
#include "Game.h"
#include "Mario.h"
 //tach collision - sua lai vu gravity 
GreenMushroom::GreenMushroom()
{
    width = MUSHROOM_WIDTH;
    height = MUSHROOM_HEIGHT;
    gravity = MUSHROOM_GRAVITY;
    vx = MUSHROOM_SPEED;
}

void GreenMushroom::SetPosition(float x, float y)
{
    CGameObject::SetPosition(x, y);
    yBegin = y;
    xBegin = x;
}

void GreenMushroom::InitAnimations()
{
    if (this->animations.size() < 1) {
        this->animations["Default"] = CAnimations::GetInstance()->Get("ani-1-up-mushroom");
    }
}

void GreenMushroom::Render()
{
    InitAnimations();
    CAnimation* ani = this->animations["Default"];

    Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
    float l, t, b, r;
    GetBoundingBox(l, t, r, b);

    ani->Render(x - camera->GetX() + (r - l) / 2, y - camera->GetY() + (b - t) / 2, flip);
}

void GreenMushroom::Update(DWORD dt)
{
   
    switch (scriptStep)
    {
    case 0:
        if (y + width <= yBegin) {
            y = yBegin - width;
            vy = 0;
            scriptStep = 1;
        }
        else {
            vy -= dt * MUSHROOM_GROWUP_SPEED;
        }
        break;
    case 1:
        gravity = MUSHROOM_GRAVITY;
        vx = MUSHROOM_SPEED;
        scriptStep = 2;
        break;
    default:
        break;
    }
    vy += gravity * dt;
    CGameObject::Update(dt);
   
}

bool GreenMushroom::CanGetThrough(CGameObject* gameObjToCollide, float coEventNx, float coEventNy)
{
    return false;
}

void GreenMushroom::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    coEvents.clear();
    if(this)
        CalcPotentialCollisions(coObjects, coEvents);    
}

void GreenMushroom::BehaviorUpdate(DWORD dt)
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
                PlayerData* pd = PlayerData::GetInstance();
                EffectVault::GetInstance()->AddEffect(new ScoreFx("1UP", x, y));
            }
        }
        break;
        }
    }
}

int GreenMushroom::GetObjectType()
{
    return ObjectType;
}
