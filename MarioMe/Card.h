#pragma once
#include "GameObject.h"

#define CARD_STAR               1
#define CARD_MUSHROOM           2
#define CARD_FLOWER             3

class Card :
    public CGameObject
{
    int cardType = CARD_STAR;

    Card(int cardType);
    virtual void Render();
    int GetCardType();
};

