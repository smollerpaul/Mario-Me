#include "Animations.h"
#include "Utils.h"
#include "Sprites.h"

CAnimation::CAnimation(int defaultTime) 
{
	this->defaultTime = defaultTime; 
	this->lastFrameTime = 0; 
	this->currentFrame = 0; 
	this->playScale = 1.0f;
	this->frames.clear();
}

void CAnimation::Add(string spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);

	if (sprite == NULL)
	{
		DebugOut(L"[ERROR] Sprite ID %d cannot be found!\n", spriteId);
	}

	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t);
	frames.push_back(frame);
}

CAnimation* CAnimation::Clone()
{
	CAnimation* result = new CAnimation(defaultTime);

	result->frames = this->frames;

	return result;
}

void CAnimation::Render(float x, float y, int flip, int alpha)
{
	DWORD now = GetTickCount64();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if ((now - lastFrameTime)*playScale > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
		}
	}

	frames[currentFrame]->GetSprite()->Draw(x, y, flip, alpha);
}

void CAnimation::SetPlayScale(float ps)
{
	this->playScale = ps;
}

CAnimations * CAnimations::__instance = NULL;

CAnimations * CAnimations::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimations();
	return __instance;
}

void CAnimations::Add(string id, LPANIMATION ani)
{
	animations[id] = ani;

	DebugOut(L"[ANIMATION] added : %s \n ", ToLPCWSTR(id));
}

LPANIMATION CAnimations::Get(string id)
{
	LPANIMATION ani = animations[id];
	if (ani == NULL)
		DebugOut(L"[ERROR] Failed to find animation id: %d\n", id);
	return ani;
}

void CAnimations::Clear()
{
	for (auto x : animations)
	{
		LPANIMATION ani = x.second;
		delete ani;
	}

	animations.clear();
}