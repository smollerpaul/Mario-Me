#include "RedMushroom.h"

void RedMushroom::InitAnimations()
{
    if (this->animations.size() < 1) {
        this->animations["Default"] = CAnimations::GetInstance()->Get("ani-super-mushroom");
    }
}
