#include "Koopas.h"
#include "Game.h"
#include "Camera.h"

CKoopas::CKoopas()
{
	SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_DIE)
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Koopas can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;

	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > 290) {
		x = 290; vx = -vx;
	}
}

void CKoopas::InitAnimations()
{
	if (this->animations.size() < 1) {
		this->animations["Move"] = CAnimations::GetInstance()->Get("ani-green-koopa-troopa-move");
	}
}

void CKoopas::Render()
{
	InitAnimations();
	CAnimation* ani = this->animations["Move"];
	if (state == KOOPAS_STATE_DIE) {
		//ani = KOOPAS_ANI_DIE;
	}
	//else if (vx > 0) ani = KOOPAS_ANI_WALKING_RIGHT;
	//else if (vx <= 0) ani = KOOPAS_ANI_WALKING_LEFT;

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	ani->Render(x- camera->GetX(), y-camera->GetY(), flip);

	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_WALKING:
		vx = KOOPAS_WALKING_SPEED;
	}

}


