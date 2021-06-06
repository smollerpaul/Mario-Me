#include <d3dx9.h>
#include <algorithm>

#include "Utils.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"
#include "Animations.h"
#include "Camera.h"

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	gravity = 0;
	nx = 1;	
	state = 0;
	dx = dy = 0;
	flip = 1;
	alive = 1;
}

void CGameObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + width;
	bottom = y + height;
}

void CGameObject::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	this->dt = dt;
	dx = vx*dt;
	dy = vy*dt;
}

void CGameObject::Render()
{
	
}

void CGameObject::UpdatePosition()
{
	x += dx;
	y += dy;
}

void CGameObject::SetPositionBottom(float x, float y)
{
	this->x = x;
	this->y = y - height;
}

bool CGameObject::CanGetThrough(CGameObject* gameObjToCollide, float coEventNx, float coEventNy)
{
	return false;
}

void CGameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	float l,t,r,b; 

	GetBoundingBox(l, t, r, b);

	rect.left = l;
	rect.top = t;
	rect.right = r;
	rect.bottom = b;

	CGame* game = CGame::GetInstance();
	Camera* cam = game->GetCurrentScene()->GetCamera();
	
	//DebugOut(L" left %f  top  %f  right %f   bottom %f", l,t,r,b);

	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get("tex-test");
	D3DXVECTOR3 pivot(0, 0, 0);
	game->Draw((x- cam->GetX() + width / 2),( y- cam->GetY()) + height/2, pivot, bbox, rect.left, rect.top, rect.right, rect.bottom, 50, flip);
}

void CGameObject::SetAlive(int alive)
{
	this->alive = alive;
}

int CGameObject::GetAlive()
{
	return alive;
}

CGameObject::~CGameObject()
{

}

void CGameObject:: SetPosition(float x, float y) { this->x = x, this->y = y; }
void CGameObject:: SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }

void CGameObject::SetSize(float w, float h){ this->width = w;	 this->height = h; }

void CGameObject:: GetPosition(float& x, float& y) { x = this->x; y = this->y; }
void CGameObject:: GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }

void CGameObject::GetSize(float& w, float& h)
{
	w = this->width;
	h = this->height;
}

int CGameObject:: GetState() { return this->state; }

int CGameObject::GetObjectType()
{
	return 0;
}
void CGameObject::SetState(int state) { this->state = state; }

void CGameObject::SetDirection(int nx)
{
	this->nx = nx;
}
int CGameObject::GetDirection()
{
	return this->nx;
}

void CGameObject::InitAnimations()
{
}



void CGameObject::SetFlipOnNormal(int nx)
{
	if (nx > 0) {
		flip = 1;
	}
	else flip = -1;
}


void CGameObject::GetPosX(float& x) {
	x = this->x;
}
void CGameObject::GetPosY(float& y) {
	y = this->y;
}

