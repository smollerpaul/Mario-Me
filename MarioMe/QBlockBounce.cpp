#include "QBlockBounce.h"
#include "QuestionBlock.h"

QBlockBounce::QBlockBounce(float x, float y, float aliveTime)
{
	Effects::SetPosition(x, y, aliveTime);
	width = height = 50;
	beginY = y;
}

void QBlockBounce::Update(DWORD dt)
{
	if (bounced == 0) {
		//DebugOut(L"chua ok \n");
		y -= 0.78 * dt;
		dyBounce += abs(0.78 * dt);
		if (dyBounce >= 50) {
			bounced = 1;
			
		}
	}
	
	if (bounced == 1) {
	//	vy += QB_GRAVITY * dt;
		y+= 0.4 * dt;
		//DebugOut(L"ok nha\n");

		if (y >= beginY) {
			y = beginY;
		}
	}

	
	aliveTimerCount += dt;
	if (aliveTimerCount >= 800) {
		alive = 0;
		bounced = 0;
	}

	//DebugOut(L"dybounce: %f\n", dyBounce);
}

void QBlockBounce::Render()
{
	if (alive == 0)
		return;

	if (this->animations.size() != 1) {
		this->animations["Default"] = CAnimations::GetInstance()->Get("ani-empty-block")->Clone();
	}

	LPANIMATION ani = this->animations["Default"];

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	int flip = 1;

	float l, t, r, b;
	GetBoundingBox(l, t, r, b);

	ani->Render(this->x - camera->GetX() + (r - l) / 2, this->y - camera->GetY() + (b - t) / 2, flip);

}
