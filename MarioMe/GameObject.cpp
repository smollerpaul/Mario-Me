#include <d3dx9.h>
#include <algorithm>

#include "Utils.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;	
	state = 0;
	dx = dy = 0;
	flip = 1;
}

void CGameObject::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	this->dt = dt;
	dx = vx*dt;
	dy = vy*dt;
}

void CGameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l,t,r,b; 

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	CGame::GetInstance()->Draw(x- camera->GetX(), y- camera->GetY(), flip, bbox, rect.left, rect.top, rect.right, rect.bottom, 32);
}

CGameObject::~CGameObject()
{

}

void CGameObject:: SetPosition(float x, float y) { this->x = x, this->y = y; }
void CGameObject:: SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }

void CGameObject:: GetPosition(float& x, float& y) { x = this->x; y = this->y; }
void CGameObject:: GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }

int CGameObject:: GetState() { return this->state; }
void CGameObject::SetState(int state) { this->state = state; }

void CGameObject:: SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }
void CGameObject::SetAnimationFlip(int nx)
{
	if (nx > 0) {
		flip = 1;
	}
	else flip = -1;
}