#include "Card.h"
#include "Camera.h"
#include "Game.h"

Card::Card(int cardType)
{
	this->cardType = cardType;
	this->SetSize(25, 30);
}

void Card::Render()
{
	if (animations.size() < 1) {
		this->animations["Flower"]= CAnimations::GetInstance()->Get("ani-goal-flower")->Clone();
		this->animations["Mushroom"] = CAnimations::GetInstance()->Get("ani-goal-mushroom")->Clone();
		this->animations["Star"] = CAnimations::GetInstance()->Get("ani-goal-star")->Clone();
	}

	LPANIMATION ani = this->animations["Star"];

	switch (cardType)
	{
	case CARD_STAR:
		ani = this->animations["Star"];
		break;

	case CARD_MUSHROOM:
		ani = this->animations["Mushroom"];
		break;

	case CARD_FLOWER:
		ani = this->animations["Flower"];
		break;
	}

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();

	ani->Render(x  + width / 2, y  + height / 2, flip);
}

int Card::GetCardType()
{
	return cardType;
}
