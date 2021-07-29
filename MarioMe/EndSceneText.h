#pragma once
#include "Effects.h"
class Text;
class EndSceneText :
    public Effects
{
public:
    Text* sentence1 = nullptr;
    Text* sentence2 = nullptr;

    int changeCard = 0;

    float timerDelay = 0;
    float timerDelayCard = 0;
    int renderSentence2 = 0;
    int renderCard = 0;

    EndSceneText(string sceneId, float x, float y, float aliveTime = 3000);
    virtual void Update(DWORD dt) override;
    virtual void Render() override;
};

