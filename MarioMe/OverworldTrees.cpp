#include "OverworldTrees.h"

OverworldTrees::OverworldTrees()
{
}

OverworldTrees::OverworldTrees(float xPos, float yPos, float aliveTime)
{
	Effects::SetPosition(xPos, yPos, aliveTime);
	width = height = 48;
}

void OverworldTrees::Update(DWORD dt)
{
}

void OverworldTrees::Render()
{
	if (this->animations.size() != 1) {
		this->animations["Default"] = CAnimations::GetInstance()->Get("ani-tree")->Clone();
	}

	LPANIMATION ani = this->animations["Default"];

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	int flip = 1;

	float l, t, r, b;
	GetBoundingBox(l, t, r, b);

	ani->Render(this->x - camera->GetX() + (r - l) / 2, this->y - camera->GetY() + (b - t) / 2, flip);
}
