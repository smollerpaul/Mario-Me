#include "HUD.h"
#include "Game.h"

void HUD::Update(DWORD dt)
{
}

void HUD::Render()
{
	CGame::GetInstance()->GetDirect3DDevice()->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
}
