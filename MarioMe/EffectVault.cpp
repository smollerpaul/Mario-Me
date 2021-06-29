#include "EffectVault.h"
#include "Effects.h"

EffectVault* EffectVault::__instance = nullptr;


EffectVault::EffectVault()
{
}

void EffectVault::Update( DWORD dt)
{
	vector<Effects*> temp;
	temp.insert(temp.end(), effects.begin(), effects.end());

	for each (Effects* fx in temp)
	{
		fx->Update(dt);
	}

	effects.erase(remove_if(effects.begin(), effects.end(), []( Effects*& fx) {
		if (fx->alive ==1) {
			return false;
		}
	}), effects.end());
}

void EffectVault::Render()
{
	for each (Effects* fx in effects)
	{
		fx->Render();
	}
}

void EffectVault::AddEffect(Effects* effect)
{
	if (effect && effect->alive==1) {
		effects.push_back(effect);
	}
}

void EffectVault::ClearEffects()
{
	this->effects.clear();
}

EffectVault* EffectVault::GetInstance()
{
	if (__instance == nullptr) {
		__instance = new EffectVault();
	}
	return __instance;
}
