#include "ArrowPicker.h"
#include "Game.h"
#include "Camera.h"

ArrowPicker::ArrowPicker()
{
	SetPosition(190,460 );
	SetSize(7, 8);
	renderOrder = 110;
}

void ArrowPicker::Update(DWORD dt)
{
}

void ArrowPicker::Render()
{
	LPSPRITE arrow = CSprites::GetInstance()->Get("spr-menu-arrow-0");
	Camera* cam = CGame::GetInstance()->GetCurrentScene()->GetCamera();

	arrow->Draw(x - cam->GetX() + width / 2, y - cam->GetY() + height / 2, 1);
}

void ArrowPicker::OnKeyUp(int keyCode)
{
}

void ArrowPicker::OnKeyDown(int keyCode)
{
	Keyboard* kb = CGame::GetInstance()->GetKeyboard();
	if (keyCode == DIK_DOWN) {
		if (y == 460) {
			y += 35;
		}
	}
	
	if (keyCode == DIK_UP) {
		if (y == 495) {
			y -=35;
		}
	}

	if (keyCode == DIK_S) {
		if (y == 460) {
			CGame::GetInstance()->SwitchScene("overworld");
		}
	}
}
